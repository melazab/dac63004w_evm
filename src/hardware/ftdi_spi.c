#include "libft4222.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// FT4222H SPI initialization
int ftdi_spi_init(FT_HANDLE *ftHandle) {
  FT4222_STATUS status;
  FT_STATUS ftStatus;
  DWORD numDevices;

  // Enumerate connected devices
  ftStatus = FT_CreateDeviceInfoList(&numDevices);
  if (ftStatus != FT_OK || numDevices == 0) {
    printf("No FTDI devices found (error %d).\n", (int)ftStatus);
    return -1;
  }

  // Open the first FTDI device
  ftStatus = FT_Open(0, ftHandle);
  if (ftStatus != FT_OK) {
    printf("Failed to open FTDI device (error %d).\n", (int)ftStatus);
    return -1;
  }

  // Initialize as SPI Master in Mode 1 (CPOL = 0, CPHA = 1)
  status = FT4222_SPIMaster_Init(*ftHandle, SPI_IO_SINGLE, CLK_DIV_16,
                                 CLK_IDLE_LOW, CLK_TRAILING, 0x01);
  if (status != FT4222_OK) {
    printf("Failed to initialize SPI master (error %d).\n", (int)status);
    FT_Close(*ftHandle);
    return -1;
  }

  // Set SPI drive strength
  status = FT4222_SPI_SetDrivingStrength(*ftHandle, DS_8MA, DS_8MA, DS_8MA);
  if (status != FT4222_OK) {
    printf("Failed to set SPI driving strength (error %d).\n", (int)status);
    FT_Close(*ftHandle);
    return -1;
  }

  printf("FTDI SPI initialized successfully in Mode 1 with CLK_DIV_16.\n");
  return 0;
}

// SPI Write Operation
int ftdi_spi_write(FT_HANDLE ftHandle, const uint8_t *data, size_t len) {
  if (!ftHandle || !data || len == 0) {
    printf("Invalid parameters for SPI write.\n");
    return -1;
  }

  uint16_t bytesTransferred;
  FT4222_STATUS status = FT4222_SPIMaster_SingleWrite(
      ftHandle, (uint8_t *)data, len, &bytesTransferred, TRUE);
  if (status != FT4222_OK || bytesTransferred != len) {
    printf("SPI write failed (error %d).\n", (int)status);
    return -1;
  }

  printf("SPI write successful: %d bytes transferred.\n", bytesTransferred);
  return 0;
}

// SPI Read Operation
int ftdi_spi_read(FT_HANDLE ftHandle, uint8_t *buffer, size_t len) {
  if (!ftHandle || !buffer || len == 0) {
    printf("Invalid parameters for SPI read.\n");
    return -1;
  }

  uint16_t bytesTransferred;
  FT4222_STATUS status = FT4222_SPIMaster_SingleRead(ftHandle, buffer, len,
                                                     &bytesTransferred, TRUE);
  if (status != FT4222_OK || bytesTransferred != len) {
    printf("SPI read failed (error %d).\n", (int)status);
    return -1;
  }

  printf("SPI read successful: %d bytes transferred.\n", bytesTransferred);
  return 0;
}

// FTDI SPI cleanup
void ftdi_spi_cleanup(FT_HANDLE ftHandle) {
  if (FT4222_UnInitialize(ftHandle) != FT4222_OK) {
    printf("Failed to uninitialize FT4222 device.\n");
  }
  FT_Close(ftHandle);
  printf("FTDI SPI cleanup complete.\n");
}
