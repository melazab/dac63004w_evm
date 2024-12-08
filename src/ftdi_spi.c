#include "ftdi_spi.h"
#include "ftdi_comm.h"
#include <stdio.h>

int ftdi_spi_init(struct ftdi_context *ftdi) {
  int ret;

  // Initialize and open FTDI device using our base functions
  if ((ret = ftdi_comm_open(ftdi)) != FTDI_COMM_SUCCESS) {
    return ret;
  }

  // Enable MPSSE mode
  if ((ret = ftdi_comm_enable_mpsse(ftdi)) != FTDI_COMM_SUCCESS) {
    return ret;
  }

  // Additional SPI-specific configuration goes here
  // [Rest of SPI configuration code...]

  return 0;
}
