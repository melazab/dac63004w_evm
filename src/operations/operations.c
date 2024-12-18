#include "operations/operations.h"
#include "utils/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int operations_init(FT_HANDLE ftHandle, dac63004w_context *ctx, float vref) {
  ctx->ftdi = ftHandle;
  return dac63004w_init(ctx, vref);
}

int handle_dc_voltage(dac63004w_context *ctx, uint8_t channel, float arg1) {
  if (!ctx || !ctx->ftdi) {
    fprintf(stderr, "Invalid DAC context.\n");
    return EXIT_FAILURE;
  }
  if (dac63004w_write_voltage(ctx, channel, arg1) != 0) {
    fprintf(stderr, "Error setting voltage on channel %d.\n", channel);
    return EXIT_FAILURE;
  }
  printf("Voltage set to %.2fV on channel %d.\n", arg1, channel);
  return 0;
}

operation_t dac_operations[] = {{"dc_voltage", handle_dc_voltage},
                                {NULL, NULL}};

operation_handler_t find_operation(const char *name) {
  for (operation_t *op = dac_operations; op->name != NULL; ++op) {
    if (strcmp(op->name, name) == 0) {
      return op->handler;
    }
  }
  return NULL;
}
