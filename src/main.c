#include "hardware/dac63004w.h"
#include "hardware/ftdi_spi.h"
#include "operations/operations.h"
/* #include "utils/signal_utils.h" */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_usage(const char *program_name);

int main(int argc, char *argv[]) {
  if (argc < 4) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }
  // Parse command-line arguments
  const char *operation = argv[1];
  uint8_t channel = atoi(argv[2]);
  float arg1 = atof(argv[3]);

  // Validate channel
  if (channel > 3) {
    fprintf(stderr, "Invalid channel: %d. Must be between 0 and 3.\n", channel);
    return EXIT_FAILURE;
  }

  // Validate argument
  if (arg1 < 0.0 || arg1 > 5.0) { // Assuming 0V to 5V range
    fprintf(stderr, "Invalid argument: %.2f. Must be between 0.0 and 5.0.\n",
            arg1);
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
  if (operations_init(ftHandle, &dac_ctx, 4.84) != DAC_SUCCESS) {
    fprintf(stderr, "DAC initialization failed.\n");
    ftdi_spi_cleanup(ftHandle);
    return EXIT_FAILURE;
  }

  // Find and run requested operation
  operation_handler_t handler = find_operation(operation);
  if (!handler) {
    fprintf(stderr, "Unknown operation: %s\n", operation);
    ftdi_spi_cleanup(ftHandle);
    return EXIT_FAILURE;
  }

  if (handler(&dac_ctx, channel, arg1) != 0) {
    fprintf(stderr, "Requested operation failed: %s\n", operation);
  }

  ftdi_spi_cleanup(ftHandle);
  return EXIT_SUCCESS;
}

void print_usage(const char *program_name) {
  fprintf(stderr, "Usage: %s <operation> <channel> <arg1>\n", program_name);
  fprintf(stderr, "Currently supported operations:\n");
  fprintf(stderr, "\tdc_voltage <channel> <voltage>\n");
}
