#ifndef FTDI_SPI_H_
#define FTDI_SPI_H_

#include <stdint.h>
#include "libft4222.h" // Required for FT_HANDLE and FT4222 functions

// Function prototypes
int ftdi_spi_init(FT_HANDLE *ftHandle);
int ftdi_spi_write(FT_HANDLE ftHandle, const uint8_t *data, size_t len);
int ftdi_spi_read(FT_HANDLE ftHandle, uint8_t *data, size_t len);
void ftdi_spi_cleanup(FT_HANDLE ftHandle);

#endif /* FTDI_SPI_H_ */
