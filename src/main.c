/* #include "dac63004w.h" */
#include "ftdi_comm.h"
/* #include <stdio.h> */
#include <stdlib.h>

int main(void) {
  struct ftdi_context *ftdi;
  int ret;

  // Initialize FTDI
  ftdi = ftdi_comm_init();
  if (!ftdi) {
    return EXIT_FAILURE;
  }

  // Open device and enable MPSSE mode
  if ((ret = ftdi_comm_open(ftdi)) != FTDI_COMM_SUCCESS) {
    ftdi_comm_cleanup(ftdi);
    return EXIT_FAILURE;
  }

  // Read the FTDI chip ID
  unsigned int chipid;
  if ((ret = ftdi_comm_read_id(ftdi, &chipid)) != FTDI_COMM_SUCCESS) {
    ftdi_comm_cleanup(ftdi);
    return EXIT_FAILURE;
  }

  if ((ret = ftdi_comm_enable_mpsse(ftdi)) != FTDI_COMM_SUCCESS) {
    ftdi_comm_cleanup(ftdi);
    return EXIT_FAILURE;
  }

  // TODO: Add DAC initialization and control code here

  // Cleanup
  ftdi_comm_cleanup(ftdi);
  return EXIT_SUCCESS;
}
