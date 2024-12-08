#ifndef DAC63004W_REGS_H_
#define DAC63004W_REGS_H_

#include <stdint.h>

// Command codes (bits 15:12)
#define DAC_CMD_NOP          (0x0 << 12)  // No operation
#define DAC_CMD_WRITE_REG    (0x1 << 12)  // Write to register
#define DAC_CMD_UPDATE       (0x2 << 12)  // Update DAC outputs
#define DAC_CMD_WRITE_UPDATE (0x3 << 12)  // Write and update
#define DAC_CMD_POWER        (0x4 << 12)  // Power up/down
#define DAC_CMD_RESET        (0x5 << 12)  // Software reset
#define DAC_CMD_LDAC        (0x6 << 12)  // LDAC mask register
#define DAC_CMD_REFERENCE   (0x7 << 12)  // Reference control
#define DAC_CMD_GAIN        (0x8 << 12)  // Gain control
#define DAC_CMD_WATCHDOG    (0x9 << 12)  // Watchdog timer
#define DAC_CMD_DAC_TEST    (0xA << 12)  // DAC test mode

// Register addresses (bits 11:8)
#define DAC_REG_DAC0        (0x0 << 8)   // DAC channel 0
#define DAC_REG_DAC1        (0x1 << 8)   // DAC channel 1
#define DAC_REG_DAC2        (0x2 << 8)   // DAC channel 2
#define DAC_REG_DAC3        (0x3 << 8)   // DAC channel 3
#define DAC_REG_ALL_DAC     (0xF << 8)   // All DAC channels

// Power modes
#define DAC_POWER_NORMAL     0x0000  // Normal operation
#define DAC_POWER_POWERDOWN  0x0001  // Power-down mode
#define DAC_POWER_HIZ        0x0002  // High-impedance mode

// Reference settings
#define DAC_REF_INTERNAL     0x0000  // Use internal reference
#define DAC_REF_EXTERNAL     0x0001  // Use external reference

// Helper macros for command building
#define DAC_BUILD_CMD(cmd, reg, val) ((cmd) | (reg) | ((val) & 0xFF))

// Utility macros for register access
#define DAC_GET_CMD(reg)     ((reg >> 12) & 0xF)
#define DAC_GET_ADDR(reg)    ((reg >> 8) & 0xF)
#define DAC_GET_DATA(reg)    (reg & 0xFF)

#endif /* DAC63004W_REGS_H_ */
