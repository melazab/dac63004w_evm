#ifndef DAC63004W_H_
#define DAC63004W_H_

#include <stdint.h>
#include "dac63004w_regs.h"
#include "libft4222.h" // Include for FT_HANDLE

// DAC context structure
typedef struct {
    FT_HANDLE ftdi;     // Handle for the FT4222H device
    double vref;        // Reference voltage
    uint8_t channel;    // Current channel
    uint8_t mode;       // Operating mode
} dac63004w_context;

// Operating modes
#define DAC_MODE_VOLTAGE    0x00
#define DAC_MODE_CURRENT    0x01

// Function return codes
#define DAC_SUCCESS         0
#define DAC_ERROR_INIT      1
#define DAC_ERROR_PARAM     2
#define DAC_ERROR_COMM      3


// Core DAC functions
int dac63004w_init(dac63004w_context *ctx, float reference_voltage);
int dac63004w_configure_channel_gain(dac63004w_context *ctx, uint8_t channel, uint16_t gain);
void dac63004w_cleanup(dac63004w_context *ctx);
int dac63004w_set_mode(dac63004w_context *ctx, uint8_t mode);

int dac63004w_write_dc_voltage(dac63004w_context *ctx, uint8_t channel, float voltage);
int dac63004w_write_dc_current(dac63004w_context *ctx, uint8_t channel, float current);
// AC waveforms
int dac63004w_write_sine_voltage(dac63004w_context *ctx, uint8_t channel, float amplitude, uint16_t frequency);
int dac63004w_write_sawtooth_voltage(dac63004w_context *ctx, uint8_t channel, float amplitude, uint16_t frequency);
int dac63004w_write_triangle_voltage(dac63004w_context *ctx, uint8_t channel, float amplitude, uint16_t frequency);

#endif /* DAC63004W_H_ */
