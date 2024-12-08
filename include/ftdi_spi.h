#ifndef FTDI_SPI_H_
#define FTDI_SPI_H_

#include <ftdi.h>
#include <stdint.h>
#include <stddef.h>

// SPI configuration
#define SPI_CLOCK_DIV      0x0000  // For 6MHz
#define SPI_CPOL           0
#define SPI_CPHA           0

// Function prototypes
int ftdi_spi_init(struct ftdi_context *ftdi);
int ftdi_spi_write(struct ftdi_context *ftdi, const uint8_t *data, size_t len);
int ftdi_spi_read(struct ftdi_context *ftdi, uint8_t *data, size_t len);
int ftdi_spi_transfer(struct ftdi_context *ftdi, const uint8_t *tx_data, 
                     uint8_t *rx_data, size_t len);

#endif /* FTDI_SPI_H_ */
