#include "hardware/dac63004w.h"
#include "hardware/ftdi_spi.h"
#include "operations/operations.h"
/* #include "utils/signal_utils.h" */ // TODO: Add error handling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VOLTAGE_MAX 4.84

void print_usage(const char *program_name);

int main(int argc, char *argv[]) {
  if ((argc < 4) || (argc > 6) || (strcmp(argv[1], "-h") == 0) ||
      (strcmp(argv[1], "--help") == 0)) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }
  // Parse command-line arguments
  const char *waveform = argv[1];
  const char *output_mode = argv[2];

  uint8_t channel = atoi(argv[3]);
  float amplitude = atof(argv[4]);
  float frequency = (argc == 6) ? atof(argv[5]) : 0.0;

  // Validate channel
  if (channel > 3) {
    fprintf(stderr, "Invalid channel: %d. Must be between 0 and 3.\n", channel);
    return EXIT_FAILURE;
  }

  // Validate argument
  if (amplitude < 0.0 || amplitude > VOLTAGE_MAX) {
    fprintf(stderr,
            "Invalid argument: %.2f. Must be between 0.0 and %.2f volts.\n",
            amplitude, VOLTAGE_MAX);
    return EXIT_FAILURE;
  }
  FT_HANDLE ftHandle;
  dac63004w_context dac_ctx;

  // Initialize FTDI SPI
  if (ftdi_spi_init(&ftHandle) != 0) {
    fprintf(stderr, "Failed to initialize FTDI SPI.\n");
    return EXIT_FAILURE;
  }

  // Initialize DAC via operations_init
  if (operations_init(ftHandle, &dac_ctx, VOLTAGE_MAX) != DAC_SUCCESS) {
    fprintf(stderr, "DAC initialization failed.\n");
    ftdi_spi_cleanup(ftHandle);
    return EXIT_FAILURE;
  }

  // Find and run requested operation
  operation_handler_t handler = find_operation(waveform, output_mode);
  if (!handler) {
    fprintf(stderr, "Unknown operation!\n");
    ftdi_spi_cleanup(ftHandle);
    return EXIT_FAILURE;
  }

  if (handler(&dac_ctx, channel, amplitude, frequency) != 0) {
    fprintf(stderr, "Requested operation failed!\n");
  }

  ftdi_spi_cleanup(ftHandle);
  return EXIT_SUCCESS;
}

void print_usage(const char *program_name) {
  // Commented lines are not yet implemented!
  fprintf(stderr,
          "Usage: %s <waveform> <output_mode> <channel> <amplitude> "
          "[<frequency>]\n",
          program_name);
  fprintf(stderr, "Currently supported DAC outputs:\n");
  fprintf(stderr, "\t dc voltage <channel> <amplitude> \n");
  /* fprintf(stderr, "\t dc current <channel> <amplitude> \n"); */
  fprintf(stderr, "\t sine voltage <channel> <amplitude> <freq> \n");
  /* fprintf(stderr, "\t sine current <channel> <amplitude> <freq> \n"); */
  /* fprintf(stderr, "\t sawtooth voltage <channel> <amplitude> <freq> \n"); */
  /* fprintf(stderr, "\t sawtooth current <channel> <amplitude> <freq> \n"); */
  /* fprintf(stderr, "\t triangle voltage <channel> <amplitude> <freq> \n"); */
  /* fprintf(stderr, "\t triangle current <channel> <amplitude> <freq> \n"); */
}
