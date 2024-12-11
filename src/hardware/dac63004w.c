#include "hardware/dac63004w.h"
#include "hardware/dac63004w_regs.h"
#include "hardware/ftdi_spi.h"
#include <stdio.h>
#include <unistd.h>

/* // Private function to write to a DAC register */

static int write_reg(dac63004w_context *ctx, uint8_t reg, uint16_t value) {
  uint8_t tx_data[3];
  // Register is already shifted in header file, shift back right to get actual
  // address
  uint8_t reg_addr = reg >> 1;
  printf("Writing DAC register 0x%02X with value 0x%04X\n", reg_addr, value);

  // Format 24-bit SPI frame:
  // [23]    R/W bit (0 for write)
  // [22:16] Register address (7 bits)
  // [15:0]  Data
  tx_data[0] = reg_addr;     // Write = 0, Address in bits 6:0
  tx_data[1] = (value >> 8); // Data MSB
  tx_data[2] = value & 0xFF; // Data LSB

  return ftdi_spi_write(ctx->ftdi, tx_data, 3);
}

int dac63004w_init(dac63004w_context *ctx, double vref) {
  int status;

  if (!ctx || vref <= 0) {
    return DAC_ERROR_PARAM;
  }

  ctx->vref = vref;
  ctx->mode = DAC_MODE_VOLTAGE;

  // Step 1: Software reset - write reset trigger to COMMON_TRIGGER register
  status = write_reg(ctx, DAC_REG_COMMON_TRIGGER, DAC_RESET_TRIGGER);
  if (status != DAC_SUCCESS) {
    printf("Reset failed\n");
    return DAC_ERROR_COMM;
  }

  usleep(1000); // 1ms delay after reset

  // Step 2: Write to COMMON-CONFIG register
  // Enable internal reference and power up all channels
  status = write_reg(ctx, DAC_REG_COMMON_CONFIG, DAC_EN_INT_REF);
  if (status != DAC_SUCCESS) {
    printf("Failed to configure COMMON-CONFIG\n");
    return DAC_ERROR_COMM;
  }

  // Step 3: Set all channels to mid-scale
  for (uint8_t channel = 0; channel < 4; channel++) {
    uint8_t reg_addr;
    switch (channel) {
    case 0:
      reg_addr = DAC_REG_DAC0_DATA;
      break;
    case 1:
      reg_addr = DAC_REG_DAC1_DATA;
      break;
    case 2:
      reg_addr = DAC_REG_DAC2_DATA;
      break;
    case 3:
      reg_addr = DAC_REG_DAC3_DATA;
      break;
    default:
      return DAC_ERROR_PARAM;
    }
    printf("Setting channel %d to mid-scale\n", channel);
    status = write_reg(ctx, reg_addr, DAC_DATA_MIDSCALE);
    if (status != DAC_SUCCESS) {
      printf("Failed to set channel %d\n", channel);
      return DAC_ERROR_COMM;
    }
  }

  return DAC_SUCCESS;
}

int dac63004w_write_voltage(dac63004w_context *ctx, uint8_t channel,
                            double voltage) {
  // Parameter validation
  if (!ctx || channel > 3 || voltage < 0 || voltage > ctx->vref) {
    return DAC_ERROR_PARAM;
  }

  // Calculate DAC code (12-bit left aligned)
  uint16_t dac_code = (uint16_t)(((voltage / ctx->vref) * 0xFFF) + 0.5) << 4;

  // Select correct register address
  uint8_t reg_addr;
  switch (channel) {
  case 0:
    reg_addr = DAC_REG_DAC0_DATA;
    break;
  case 1:
    reg_addr = DAC_REG_DAC1_DATA;
    break;
  case 2:
    reg_addr = DAC_REG_DAC2_DATA;
    break;
  case 3:
    reg_addr = DAC_REG_DAC3_DATA;
    break;
  default:
    return DAC_ERROR_PARAM;
  }

  printf("Setting channel %d to %.3fV (code: 0x%04X)\n", channel, voltage,
         dac_code);

  return write_reg(ctx, reg_addr, dac_code);
}
