#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "hardware/dac63004w.h" // Required for dac63004w_context type
#include "hardware/ftdi_spi.h" // Required for FT_HANDLE type

// Initialize DAC for operations
int operations_init(FT_HANDLE ftHandle, dac63004w_context *ctx, float vref);

// Function pointer type for operations
typedef int (*operation_handler_t)(dac63004w_context *ctx, uint8_t channel, float arg1);

// Find an operation handler by name
operation_handler_t find_operation(const char *name);

// Operation structure
typedef struct {
    const char *name;
    operation_handler_t handler;
} operation_t;

#endif // OPERATIONS_H
