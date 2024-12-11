#ifndef DAC63004W_H_
#define DAC63004W_H_

#include <ftdi.h>
#include <stdint.h>
#include "dac63004w_regs.h"

// DAC context structure
typedef struct {
    struct ftdi_context *ftdi;
    double vref;        // Reference voltage
    uint8_t channel;    // Current channel
    uint8_t mode;       // Operating mode
} dac63004w_context;

// Operating modes
#define DAC_MODE_VOLTAGE    0x00
#define DAC_MODE_CURRENT    0x01

// Function return codes
#define DAC_SUCCESS         0
#define DAC_ERROR_INIT     -1
#define DAC_ERROR_PARAM    -2
#define DAC_ERROR_COMM     -3

// Core DAC functions
int dac63004w_init(dac63004w_context *ctx, double vref);
void dac63004w_cleanup(dac63004w_context *ctx);
int dac63004w_set_mode(dac63004w_context *ctx, uint8_t mode);
int dac63004w_write_voltage(dac63004w_context *ctx, uint8_t channel, double voltage);
int dac63004w_write_current(dac63004w_context *ctx, uint8_t channel, double current_ma);

#endif /* DAC63004W_H_ */
