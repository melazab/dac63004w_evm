#ifndef AC_CURRENT_H
#define AC_CURRENT_H

#include "hardware/dac63004w.h"

// Handle AC current output
int handle_ac_current(dac63004w_context *ctx, uint8_t channel, float amplitude, float frequency);

#endif // AC_CURRENT_H
