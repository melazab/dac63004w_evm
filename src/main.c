#include "hardware/dac63004w.h"
#include "hardware/ftdi_comm.h"
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
  struct ftdi_context *ftdi;
  dac63004w_context dac_ctx;
  int status;

  // Initialize FTDI
  ftdi = ftdi_comm_init();
  if (!ftdi) {
    printf("Failed to initialize FTDI context\n");
    return -1;
  }

  // Open FTDI device
  status = ftdi_comm_open(ftdi);
  if (status != FTDI_COMM_SUCCESS) {
    printf("Failed to open FTDI device\n");
    ftdi_comm_cleanup(ftdi);
    return -1;
  }

  // Read chip ID for verification
  unsigned int chipid;
  status = ftdi_comm_read_id(ftdi, &chipid);
  if (status != FTDI_COMM_SUCCESS) {
    printf("Failed to read FTDI chip ID\n");
    ftdi_comm_cleanup(ftdi);
    return -1;
  }

  // Enable MPSSE mode
  status = ftdi_comm_enable_mpsse(ftdi);
  if (status != FTDI_COMM_SUCCESS) {
    printf("Failed to enable MPSSE mode\n");
    ftdi_comm_cleanup(ftdi);
    return -1;
  }

  // Initialize SPI
  status = ftdi_spi_init(ftdi);
  if (status != 0) {
    printf("Failed to initialize SPI: %d\n", status);
    ftdi_comm_cleanup(ftdi);
    return -1;
  }

  // Initialize DAC with 3.3V reference
  dac_ctx.ftdi = ftdi;
  status = dac63004w_init(&dac_ctx, 3.3);
  if (status != DAC_SUCCESS) {
    printf("DAC initialization failed: %d\n", status);
    ftdi_comm_cleanup(ftdi);
    return -1;
  }

  printf("DAC initialized successfully!\n");

  // Set up signal handler for Ctrl+C
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  printf("Running... Press Ctrl+C to exit\n");

  // Main loop
  while (running) {

    // Check FTDI status by purging both buffers
    if (ftdi_tciflush(ftdi) < 0 || ftdi_tcoflush(ftdi) < 0) {
      printf("FTDI buffer purge error: %s\n", ftdi_get_error_string(ftdi));
      break;
    } // Set voltage on each channel to mid-scale voltage
    for (int channel = 0; channel < 4; channel++) {
      status = dac63004w_write_voltage(&dac_ctx, channel, 1.65);
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

  ftdi_comm_cleanup(ftdi);
  printf("Cleanup complete\n");
  return 0;
}
