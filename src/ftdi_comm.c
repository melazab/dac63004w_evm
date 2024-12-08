#include "ftdi_comm.h"
#include <stdio.h>
#include <stdlib.h>

struct ftdi_context *ftdi_comm_init(void) {
  struct ftdi_context *ftdi = ftdi_new();
  if (ftdi == NULL) {
    fprintf(stderr, "ftdi_new failed\n");
    return NULL;
  }
  return ftdi;
}

int ftdi_comm_open(struct ftdi_context *ftdi) {
  int ret = ftdi_usb_open(ftdi, FTDI_VID, FTDI_PID_FT4222);
  if (ret < 0) {
    fprintf(stderr, "Unable to open FTDI device: %d (%s)\n", ret,
            ftdi_get_error_string(ftdi));
    return FTDI_COMM_ERR_OPEN;
  }
  printf("FTDI device opened successfully\n");

  return FTDI_COMM_SUCCESS;
}

int ftdi_comm_read_id(struct ftdi_context *ftdi, unsigned int *chipid) {
  int ret = ftdi_read_chipid(ftdi, chipid);
  if (ret < 0) {
    fprintf(stderr, "Unable to read chip ID: %d (%s)\n", ret,
            ftdi_get_error_string(ftdi));
    return FTDI_COMM_ERR_READ;
  }
  printf("FTDI chip ID: 0x%X\n", *chipid);
  return FTDI_COMM_SUCCESS;
}

int ftdi_comm_enable_mpsse(struct ftdi_context *ftdi) {
  int ret = ftdi_set_bitmode(ftdi, 0x00, BITMODE_MPSSE);
  if (ret < 0) {
    fprintf(stderr, "Unable to set MPSSE mode: %d (%s)\n", ret,
            ftdi_get_error_string(ftdi));
    return FTDI_COMM_ERR_MODE;
  }
  printf("MPSSE mode enabled\n");
  return FTDI_COMM_SUCCESS;
}

void ftdi_comm_cleanup(struct ftdi_context *ftdi) {
  if (ftdi) {
    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);
  }
}
