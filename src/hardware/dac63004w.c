#include "hardware/dac63004w.h"
#include "hardware/dac63004w_regs.h"
#include "hardware/ftdi_spi.h"
#include <stdio.h>
#include <unistd.h>

/* Helper Functions */
// Helper function to select DAC register based on channel
static uint8_t get_dac_register(uint8_t channel) {
  switch (channel) {
  case 0:
    return DAC_REG_DAC0_DATA;
  case 1:
    return DAC_REG_DAC1_DATA;
  case 2:
    return DAC_REG_DAC2_DATA;
  case 3:
    return DAC_REG_DAC3_DATA;
  default:
    return 0xFF; // Invalid channel
  }
}

// Helper function to write to a DAC register
static int write_reg(dac63004w_context *ctx, uint8_t reg, uint16_t value) {
  if (!ctx || reg == 0xFF) {
    return DAC_ERROR_PARAM;
  }

  uint8_t tx_data[3];
  tx_data[0] = reg;          // Register address (7 bits)
  tx_data[1] = (value >> 8); // Data MSB
  tx_data[2] = value & 0xFF; // Data LSB

  printf("Writing DAC register 0x%02X with value 0x%04X\n", reg, value);

  int status = ftdi_spi_write(ctx->ftdi, tx_data, sizeof(tx_data));
  if (status != 0) {
    printf("SPI write error: %d\n", status);
    return DAC_ERROR_COMM;
  }

  return DAC_SUCCESS;
}

// DAC initialization
int dac63004w_init(dac63004w_context *ctx, double vref) {
  if (!ctx || vref <= 0) {
    return DAC_ERROR_PARAM;
  }

  ctx->vref = vref;
  ctx->mode = DAC_MODE_VOLTAGE;

  // Step 1: Software reset
  if (write_reg(ctx, DAC_REG_COMMON_TRIGGER, DAC_RESET_TRIGGER) !=
      DAC_SUCCESS) {
    printf("DAC reset failed\n");
    return DAC_ERROR_COMM;
  }
  usleep(1000); // Wait for reset to complete

  // Step 2: Configure gain settings for each channel
  for (uint8_t channel = 0; channel < 4; channel++) {
    uint8_t reg_addr = 0x03 + channel; // DAC-X-VOUT-CMP-CONFIG
    if (write_reg(ctx, reg_addr, 0x0800) != DAC_SUCCESS) {
      printf("Failed to configure gain for channel %d\n", channel);
      return DAC_ERROR_COMM;
    }
  }

  // Step 3: Enable internal reference and set normal operation
  if (write_reg(ctx, DAC_REG_COMMON_CONFIG, 0x1249) != DAC_SUCCESS) {
    printf("Failed to configure DAC common settings\n");
    return DAC_ERROR_COMM;
  }

  // Step 4: Set all channels to mid-scale
  for (uint8_t channel = 0; channel < 4; channel++) {
    uint8_t reg_addr = get_dac_register(channel);
    if (reg_addr == 0xFF ||
        write_reg(ctx, reg_addr, DAC_DATA_MIDSCALE) != DAC_SUCCESS) {
      printf("Failed to set channel %d to mid-scale\n", channel);
      return DAC_ERROR_COMM;
    }
  }

  // Step 5: Trigger LDAC to update outputs
  if (write_reg(ctx, DAC_REG_COMMON_TRIGGER, DAC_LDAC_TRIGGER) != DAC_SUCCESS) {
    printf("Failed to trigger LDAC\n");
    return DAC_ERROR_COMM;
  }

  printf("DAC initialized successfully\n");
  return DAC_SUCCESS;
}

// Write voltage to a specific DAC channel
int dac63004w_write_voltage(dac63004w_context *ctx, uint8_t channel,
                            double voltage) {
  if (!ctx || channel > 3 || voltage < 0 || voltage > ctx->vref) {
    return DAC_ERROR_PARAM;
  }

  uint16_t dac_code = (uint16_t)(((voltage / ctx->vref) * 0xFFF) + 0.5) << 4;
  uint8_t reg_addr = get_dac_register(channel);

  if (reg_addr == 0xFF) {
    return DAC_ERROR_PARAM;
  }

  printf("Setting channel %d to %.3fV (code: 0x%04X)\n", channel, voltage,
         dac_code);
  return write_reg(ctx, reg_addr, dac_code);
}
