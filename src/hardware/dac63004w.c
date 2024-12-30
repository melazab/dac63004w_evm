#include "hardware/dac63004w.h"
#include "hardware/dac63004w_regs.h"
#include "hardware/ftdi_spi.h"
#include <stdio.h>
#include <unistd.h>

typedef struct {
  uint16_t frequency;    // Hz
  slew_rate_t slew_rate; // µs/step
} freq_slew_rate_map_t;

// 15 supported frequencies (see dac63004w.h) in func gen mode
static const freq_slew_rate_map_t freq_slew_map[] = {
    {10417, DAC_SLEW_RATE_4_US}, {5208, DAC_SLEW_RATE_8_US},
    {3472, DAC_SLEW_RATE_12_US}, {2315, DAC_SLEW_RATE_18_US},
    {1541, DAC_SLEW_RATE_27_US}, {1029, DAC_SLEW_RATE_40_US},
    {699, DAC_SLEW_RATE_61_US},  {467, DAC_SLEW_RATE_91_US},
    {304, DAC_SLEW_RATE_137_US}, {174, DAC_SLEW_RATE_239_US},
    {101, DAC_SLEW_RATE_418_US}, {57, DAC_SLEW_RATE_733_US},
    {33, DAC_SLEW_RATE_1282_US}, {16, DAC_SLEW_RATE_2564_US},
    {8, DAC_SLEW_RATE_5128_US}};

/* Helper Functions */
static slew_rate_t get_slew_rate(uint16_t frequency) {
  for (int i = 0; i < 15; i++)
    if (frequency == freq_slew_map[i].frequency)
      return freq_slew_map[i].slew_rate;

  return DAC_SLEW_RATE_NONE;
}
static void print_supported_freqs(void) {
  fprintf(stderr,
          "Here are the supported frequencies for the DAC63004 outputs: \n");
  for (int i = 0; i < 15; i++)
    fprintf(stderr, "\t%d\n", freq_slew_map[i].frequency);
}
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

static int write_reg(dac63004w_context *ctx, uint8_t reg, uint16_t value) {
  if (!ctx || reg == 0xFF) {
    return DAC_ERROR_PARAM;
  }

  uint8_t tx_data[3];
  tx_data[0] = (reg & 0x7F);        // Register address (7 bits) + R/W=0
  tx_data[1] = (value >> 8) & 0xFF; // Data MSB
  tx_data[2] = value & 0xFF;        // Data LSB

  printf("Writing to DAC register 0x%02X with value 0x%04X\n", reg, value);

  int status = ftdi_spi_write(ctx->ftdi, tx_data, sizeof(tx_data));
  if (status != 0) {
    printf("SPI write error: %d\n", status);
    return DAC_ERROR_COMM;
  }

  return DAC_SUCCESS;
}
// DAC initialization
int dac63004w_init(dac63004w_context *ctx, float reference_voltage) {
  if (!ctx || reference_voltage <= 0) {
    return DAC_ERROR_PARAM;
  }

  ctx->vref = reference_voltage;
  ctx->mode = DAC_MODE_VOLTAGE;

  // Step 1: Software reset
  if (write_reg(ctx, DAC_REG_COMMON_TRIGGER, DAC_RESET_TRIGGER) !=
      DAC_SUCCESS) {
    printf("DAC reset failed\n");
    return DAC_ERROR_COMM;
  }
  usleep(1000); // Wait for reset to complete

  // Step 2: Initialize default gain settings for each channel
  uint16_t default_gain = DAC_VOUT_GAIN_1X_VDD_REFERENCE;
  for (uint8_t channel = 0; channel < 4; channel++) {
    uint8_t reg_addr = 0x03 + (channel * 6); // DAC_X_VOUT_CMP_CONFIG
    if (write_reg(ctx, reg_addr, default_gain) != DAC_SUCCESS) {
      printf("Failed to configure gain for channel %d\n", channel);
      return DAC_ERROR_COMM;
    }
    printf("Configuring gain for channel %d: reg_addr=0x%02X, value=0x%04X\n",
           channel, reg_addr, default_gain);
  }

  // Step 3: Enable internal reference and set normal operation
  if (write_reg(ctx, DAC_REG_COMMON_CONFIG, 0x1249) != DAC_SUCCESS) {
    fprintf(stderr, "Failed to configure DAC common settings\n");
    return DAC_ERROR_COMM;
  }

  // Step 4: Trigger LDAC to update outputs
  if (write_reg(ctx, DAC_REG_COMMON_TRIGGER, DAC_LDAC_TRIGGER) != DAC_SUCCESS) {
    fprintf(stderr, "Failed to trigger LDAC\n");
    return DAC_ERROR_COMM;
  }

  printf("DAC initialized successfully\n");
  return DAC_SUCCESS;
}

// Write voltage to a specific DAC channel
int dac63004w_write_dc_voltage(dac63004w_context *ctx, uint8_t channel,
                               float voltage) {
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

// Write current output to a specific DAC channel
int dac63004w_write_dc_current(dac63004w_context *ctx, uint8_t channel,
                               float current) {
  return DAC_ERROR_PARAM;
}
int dac63004w_configure_channel_gain(dac63004w_context *ctx, uint8_t channel,
                                     uint16_t config_value) {
  if (!ctx || channel > 3) {
    return DAC_ERROR_PARAM;
  }

  uint8_t reg_addr = 0x03 + (channel * 6); // DAC-X-VOUT-CMP-CONFIG
  return write_reg(ctx, reg_addr, config_value);
}

// Write sine voltage output to a specific DAC channel
int dac63004w_write_sine_voltage(dac63004w_context *ctx, uint8_t channel,
                                 float amplitude, uint16_t frequency) {
  if (!ctx || channel > 3 || amplitude <= 0 || frequency <= 0 ||
      amplitude > ctx->vref) {
    return DAC_ERROR_PARAM;
  }
  slew_rate_t selected_slew_rate = get_slew_rate(frequency);

  if (selected_slew_rate == DAC_SLEW_RATE_NONE) {
    fprintf(stderr,
            "Invalid frequency %u Hz. Supported range is 8 Hz to 10417 Hz.\n",
            frequency);
    print_supported_freqs();
    return DAC_ERROR_PARAM;
  }
  // Configure the DAC-X-FUNC-CONFIG register for sine wave generation
  uint8_t func_config_reg = DAC_REG_DAC0_FUNC_CONFIG +
                            (channel * 6); // Offset for FUNC-X-CONFIG registers
  uint16_t func_config_value = DAC_FUNC_CONFIG_SINE | selected_slew_rate;
  if (write_reg(ctx, func_config_reg, func_config_value) != DAC_SUCCESS) {
    fprintf(stderr, "Failed to configure channel %d with sine wave output\n",
            channel);
    return DAC_ERROR_COMM;
  }
  // Start function generation by writing to COMMON-DAC-TRIG
  uint16_t start_func_gen = START_FUNC_GEN << (12 - 4 * channel);
  if (write_reg(ctx, DAC_REG_COMMON_DAC_TRIG, start_func_gen) != DAC_SUCCESS) {
    fprintf(stderr, "Failed to start function generation!\n");
    return DAC_ERROR_COMM;
  }
  return DAC_SUCCESS;
}

// Write sawtooth voltage output to a specific DAC channel
int dac63004w_write_sawtooth_voltage(dac63004w_context *ctx, uint8_t channel,
                                     float amplitude, uint16_t frequency) {
  if (!ctx || channel > 3 || amplitude <= 0 || frequency <= 0 ||
      amplitude > ctx->vref) {
    return DAC_ERROR_PARAM;
  }

  return DAC_SUCCESS;
}

int dac63004w_write_triangle_voltage(dac63004w_context *ctx, uint8_t channel,
                                     float amplitude, uint16_t frequency) {
  if (!ctx || channel > 3 || amplitude <= 0 || frequency <= 0 ||
      amplitude > ctx->vref) {
    return DAC_ERROR_PARAM;
  }

  return DAC_SUCCESS;
}
