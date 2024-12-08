#ifndef FTDI_COMM_H_
#define FTDI_COMM_H_

#include <ftdi.h>
#include <stdint.h>

// Function return codes
#define FTDI_COMM_SUCCESS     0
#define FTDI_COMM_ERR_INIT   -1
#define FTDI_COMM_ERR_OPEN   -2
#define FTDI_COMM_ERR_READ   -3
#define FTDI_COMM_ERR_MODE   -4

// FTDI device identifiers
#define FTDI_VID        0x0403    // FTDI Vendor ID
#define FTDI_PID_FT4222 0x601C    // FT4222H Product ID

// Basic FTDI communication functions
struct ftdi_context* ftdi_comm_init(void);
int ftdi_comm_open(struct ftdi_context *ftdi);
int ftdi_comm_read_id(struct ftdi_context *ftdi, unsigned int *chipid);
int ftdi_comm_enable_mpsse(struct ftdi_context *ftdi);
void ftdi_comm_cleanup(struct ftdi_context *ftdi);

#endif /* FTDI_COMM_H_ */
