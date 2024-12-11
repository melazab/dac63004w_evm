#include "hardware/ftdi_spi.h"
#include <ftdi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// MPSSE command definitions
#define MPSSE_WRITE_BYTES 0x10
#define MPSSE_MSB_FIRST 0x00
#define MPSSE_CS_HIGH 0x0C
#define MPSSE_CS_LOW 0x0B
#define MPSSE_WAIT 0x86

int ftdi_spi_init(struct ftdi_context *ftdi) {
  if (!ftdi) {
    return -1;
  }

  int ret;
  uint8_t setup_sequence[] = {
      0x80, 0x00, 0x0B, // Set initial state: all outputs low
                        // Direction: bit0=SCK, bit1=MOSI, bit2=CS as outputs
                        // (0x0B = 0b1011)
      0x86, 0x00, 0x00, // Delay 1us to ensure stable state
      0x8A,             // Use 60MHz master clock
      0x97, 0x96,       // Set clock divisor to 12 (5MHz)
      0x8D,             // Disable adaptive clocking
      0x85,             // Disable 3-phase clocking
      0x80, 0x00, 0x0B  // Ensure outputs are still correctly set
  };

  printf("Initializing FTDI SPI...\n");

  // Reset the MPSSE
  ret = ftdi_set_bitmode(ftdi, 0x00, BITMODE_RESET);
  if (ret < 0) {
    printf("Failed to reset MPSSE: %s\n", ftdi_get_error_string(ftdi));
    return ret;
  }
  usleep(10000); // 10ms delay

  // Enable MPSSE mode
  ret = ftdi_set_bitmode(ftdi, 0x00, BITMODE_MPSSE);
  if (ret < 0) {
    printf("Failed to enable MPSSE mode: %s\n", ftdi_get_error_string(ftdi));
    return ret;
  }
  usleep(10000); // 10ms delay

  // Send setup sequence
  ret = ftdi_write_data(ftdi, setup_sequence, sizeof(setup_sequence));
  if (ret < 0 || ret != sizeof(setup_sequence)) {
    printf("Failed to send setup commands: %d\n", ret);
    return ret;
  }

  usleep(10000); // 10ms delay after initialization
  printf("SPI initialization complete\n");
  return 0;
}

int ftdi_spi_write(struct ftdi_context *ftdi, const uint8_t *data, size_t len) {
  if (!ftdi || !data || len != 3) { // Must be exactly 3 bytes (24 bits)
    return -1;
  }

  printf("SPI Frame: ");
  for (size_t i = 0; i < len; i++) {
    printf("%02X ", data[i]);
  }
  printf("\n");

  // Calculate total buffer size needed: 3 bytes start + 3 bytes data + 3 bytes
  // end
  uint8_t buffer[9]; // Changed from 7 to 9
  size_t idx = 0;

  // Set CS low and start clock data out - combined command
  buffer[idx++] = 0x90; // Command: set GPIO + clock data out
  buffer[idx++] = 0x00; // Value: Set SYNC low
  buffer[idx++] = 0x0B; // Direction

  // 24 bits of data (3 bytes)
  memcpy(&buffer[idx], data, 3);
  idx += 3;

  // Set CS high after complete transfer
  buffer[idx++] = 0x80; // Command: set GPIO
  buffer[idx++] = 0x04; // Value: Set SYNC high
  buffer[idx++] = 0x0B; // Direction

  printf("FTDI buffer contents (%zu bytes): ", idx);
  for (size_t i = 0; i < idx; i++) {
    printf("%02X ", buffer[i]);
  }
  printf("\n");

  // Send entire command sequence
  int ret = ftdi_write_data(ftdi, buffer, idx);

  return (ret < 0 || ret != idx) ? -1 : 0;
}

// Placeholder for future implementation
int ftdi_spi_transfer(struct ftdi_context *ftdi, const uint8_t *tx_data,
                      uint8_t *rx_data, size_t len) {
  (void)ftdi;    // Suppress unused parameter warning
  (void)tx_data; // Suppress unused parameter warning
  (void)rx_data; // Suppress unused parameter warning
  (void)len;     // Suppress unused parameter warning
  return -1;     // Not implemented yet
}
