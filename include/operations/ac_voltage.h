#ifndef AC_VOLTAGE_H
#define AC_VOLTAGE_H

#include "hardware/dac63004w.h"

// Handle AC voltage output
int handle_ac_voltage(dac63004w_context *ctx, uint8_t channel, float amplitude, float frequency);

#endif // AC_VOLTAGE_H
