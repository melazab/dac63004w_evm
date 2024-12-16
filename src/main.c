#include "hardware/dac63004w.h"
#include "hardware/ftdi_spi.h"
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

static volatile bool running = true;

void signal_handler(int signum) {
  (void)signum; // Unused parameter
  running = false;
  printf("\nReceived shutdown signal. Cleaning up...\n");
}

int main(void) {
  FT_HANDLE ftHandle; // FT_HANDLE declared in libft4222.h
  dac63004w_context dac_ctx;
  int status;

  // Initialize FTDI SPI
  if (ftdi_spi_init(&ftHandle) != 0) {
    printf("Failed to initialize FTDI SPI.\n");
    return -1;
  }

  // Initialize DAC with 3.3V reference
  dac_ctx.ftdi = ftHandle;
  status = dac63004w_init(&dac_ctx, 3.3);
  if (status != DAC_SUCCESS) {
    printf("DAC initialization failed: %d\n", status);
    ftdi_spi_cleanup(ftHandle);
    return -1;
  }

  printf("DAC initialized successfully!\n");

  // Set up signal handler for Ctrl+C
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  printf("Running... Press Ctrl+C to exit\n");

  // Main loop
  while (running) {
    // Set voltage on each channel to mid-scale voltage
    for (int channel = 0; channel < 4; channel++) {
      status = dac63004w_write_voltage(&dac_ctx, channel, 1.00);
      if (status != DAC_SUCCESS) {
        printf("Failed to set voltage on channel %d\n", channel);
      }
    }
    usleep(10000); // 10 ms delay
  }

  // Cleanup - set all channels to 0V
  printf("Setting all channels to 0V...\n");
  for (int channel = 0; channel < 4; channel++) {
    dac63004w_write_voltage(&dac_ctx, channel, 0.0);
  }

  ftdi_spi_cleanup(ftHandle);
  printf("Cleanup complete\n");
  return 0;
}
