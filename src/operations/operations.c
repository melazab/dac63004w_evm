#include "operations/operations.h"
#include "utils/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int operations_init(FT_HANDLE ftHandle, dac63004w_context *ctx,
                    float reference_voltage) {
  ctx->ftdi = ftHandle;
  return dac63004w_init(ctx, reference_voltage);
}

int handle_dc_voltage(dac63004w_context *ctx, uint8_t channel, float amplitude,
                      uint16_t frequency) {
  // Ignore frequency argument for DC
  if (!ctx || !ctx->ftdi) {
    fprintf(stderr, "Invalid DAC context.\n");
    return EXIT_FAILURE;
  }
  if (dac63004w_write_dc_voltage(ctx, channel, amplitude) != 0) {
    fprintf(stderr, "Error setting voltage on channel %d.\n", channel);
    return EXIT_FAILURE;
  }
  printf("Voltage set to %.2f V on channel %d.\n", amplitude, channel);
  return 0;
}
int handle_dc_current(dac63004w_context *ctx, uint8_t channel, float amplitude,
                      uint16_t frequency) {
  // Ignore frequency argument for DC
  if (!ctx || !ctx->ftdi) {
    fprintf(stderr, "Invalid DAC context.\n");
    return EXIT_FAILURE;
  }
  if (dac63004w_write_dc_current(ctx, channel, amplitude) != 0) {
    fprintf(stderr, "Error setting voltage on channel %d.\n", channel);
    return EXIT_FAILURE;
  }
  printf("Current set to %.2f mA on channel %d.\n", amplitude, channel);
  return 0;
}

int handle_sine_voltage(dac63004w_context *ctx, uint8_t channel,
                        float amplitude, uint16_t frequency) {
  if (!ctx || !ctx->ftdi) {
    fprintf(stderr, "Invalid DAC context.\n");
    return EXIT_FAILURE;
  }
  fprintf(stdout, "\nConfiguring DAC channel %d with sine voltage...\n",
          channel);
  if (dac63004w_write_sine_voltage(ctx, channel, amplitude, frequency) != 0) {
    fprintf(stderr, "Error setting voltage on channel %d.\n", channel);
    return EXIT_FAILURE;
  }
  printf("Voltage => %.2f V, sine, %d Hz set on channel %d.\n", amplitude,
         frequency, channel);
  return EXIT_SUCCESS;
}
int handle_sawtooth_voltage(dac63004w_context *ctx, uint8_t channel,
                            float amplitude, uint16_t frequency) {
  if (!ctx || !ctx->ftdi) {
    fprintf(stderr, "Invalid DAC context.\n");
    return EXIT_FAILURE;
  }
  if (dac63004w_write_sawtooth_voltage(ctx, channel, amplitude, frequency) !=
      0) {
    fprintf(stderr, "Error setting voltage on channel %d.\n", channel);
    return EXIT_FAILURE;
  }
  printf("Voltage => %.2f V, sawtooth, %d Hz set on channel %d.\n", amplitude,
         frequency, channel);
  return EXIT_SUCCESS;
}
int handle_triangle_voltage(dac63004w_context *ctx, uint8_t channel,
                            float amplitude, uint16_t frequency) {
  if (!ctx || !ctx->ftdi) {
    fprintf(stderr, "Invalid DAC context.\n");
    return EXIT_FAILURE;
  }
  if (dac63004w_write_triangle_voltage(ctx, channel, amplitude, frequency) !=
      0) {
    fprintf(stderr, "Error setting voltage on channel %d.\n", channel);
    return EXIT_FAILURE;
  }
  printf("Voltage => %.2f V, triangle, %d Hz set on channel %d.\n", amplitude,
         frequency, channel);
  return EXIT_SUCCESS;
}

int handle_sine_current(dac63004w_context *ctx, uint8_t channel,
                        float amplitude, uint16_t frequency) {
  printf("Here is a sine current waveform! \n");
  return EXIT_SUCCESS;
}

int handle_sawtooth_current(dac63004w_context *ctx, uint8_t channel,
                            float amplitude, uint16_t frequency) {
  printf("Here is a sawtooth current waveform! \n");
  return EXIT_SUCCESS;
}
int handle_triangle_current(dac63004w_context *ctx, uint8_t channel,
                            float amplitude, uint16_t frequency) {
  printf("Here is a triangular current waveform! \n");
  return EXIT_SUCCESS;
}
operation_t operations[] = {{"dc", "voltage", handle_dc_voltage},
                            {"dc", "current", handle_dc_current},
                            {"sine", "voltage", handle_sine_voltage},
                            {"sine", "current", handle_sine_current},
                            {"sawtooth", "voltage", handle_sawtooth_voltage},
                            {"sawtooth", "current", handle_sawtooth_current},
                            {"triangle", "voltage", handle_triangle_voltage},
                            {"triangle", "current", handle_triangle_current},
                            {NULL, NULL, NULL}};

operation_handler_t find_operation(const char *waveform,
                                   const char *output_mode) {
  for (operation_t *op = operations; op->waveform != NULL; ++op) {
    if ((strcmp(op->waveform, waveform) == 0) &&
        (strcmp(op->output_mode, output_mode) == 0)) {
      return op->handler;
    }
  }
  return NULL;
}
