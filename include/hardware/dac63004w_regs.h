#ifndef DAC63004W_REGS_H_
#define DAC63004W_REGS_H_
#include <stdint.h>

// DAC data registers
#define DAC_REG_DAC0_DATA      (0x19 << 1)    // DAC channel 0 data register
#define DAC_REG_DAC1_DATA      (0x1A << 1)    // DAC channel 1 data register
#define DAC_REG_DAC2_DATA      (0x1B << 1)    // DAC channel 2 data register
#define DAC_REG_DAC3_DATA      (0x1C << 1)    // DAC channel 3 data register

// Configuration registers
#define DAC_REG_COMMON_CONFIG  (0x1F << 1)    // Common configuration register
#define DAC_REG_COMMON_TRIGGER (0x20 << 1)    // Common trigger register
#define DAC_REG_DEVICE_MODE    (0x25 << 1)    // Device mode configuration
#define DAC_REG_INTERFACE      (0x26 << 1)    // Interface configuration

// Common config register bits
#define DAC_EN_INT_REF         (1 << 12)   // Enable internal reference
#define DAC_VOUT_PDN_MASK     (0x3 << 10)  // Voltage output power down mask
#define DAC_VOUT_PDN_NORMAL   0x00         // Normal operation
#define DAC_VOUT_PDN_HIZ      0x03         // Hi-Z to AGND

// Common trigger register bits
#define DAC_RESET_TRIGGER     (0xA << 8)   // Reset trigger value
#define DAC_LDAC_TRIGGER      (1 << 7)     // LDAC trigger bit
#define DAC_CLR_TRIGGER       (1 << 6)     // Clear trigger bit

// Device mode config bits
#define DAC_MODE_NORMAL       0x00         // Normal operation mode
#define DAC_MODE_LOWPOWER     (1 << 13)    // Low power mode

// Helper macros for DAC data formatting
#define DAC_DATA_12BIT(val)   ((val & 0xFFF) << 4)  // 12-bit data left aligned
#define DAC_DATA_MIDSCALE     0x8000              // Mid-scale value (0x800 << 4)

#endif /* DAC63004W_REGS_H_ */
