#include "types.h"

struct int_type {
  volatile uint32_t INT_CTRLR_TYPE;  // Interrupt Control Type Register
  volatile uint32_t AUX_CTRL;       // Auxiliary Control Register
};
#define NVIC_BASE   0xE000E000
#define INTTYPE     ((struct int_type *)(NVIC_BASE + 0x4))

// ============ Interrupt Controller Type Register ===============
// Access: Read-only
// Reset state: N/A
//
// Read to see the number of interrupt lines that NVIC supports.

#define INT_LINES_NUM_SHIFT(n)    ((n) << 0)
#define INT_LINES_NUM_MASK        INT_LINES_NUM_SHIFT(0x1F)
enum INT_LINES_NUM_OPTS {
  INT_LINES_NUM_1_32 = 0, // b00000 = 0  ..32  (a)
  INT_LINES_NUM_33_64,    // b00001 = 33 ..64
  INT_LINES_NUM_65_96,    // b00010 = 65 ..96
  INT_LINES_NUM_97_128,   // b00011 = 97 ..128
  INT_LINES_NUM_129_160,  // b00100 = 129..160
  INT_LINES_NUM_161_192,  // b00101 = 161..192
  INT_LINES_NUM_193_224,  // b00110 = 193..224
  INT_LINES_NUM_225_240,  // b00111 = 225..256 (a)
};
// a - The processor only supports between 1 and 240 external interrupts.

// ============= Auxilliary Control Register ====================
// Access: Read/Write
// Reset state: 0x0000_0000
//
// Disable certain aspect of functionality within processor.

#define DISFOLD_SHIFT(n)      ((n) << 2)
#define DISFOLD               DISFOLD_SHIFT(1)

#define DISDEFWBUF_SHIFT(n)   ((n) << 1)
#define DISDEFWBUF            DISDEFWBUF_SHIFT(1)

#define DISMCYCINT_SHIFT(n)   ((n) << 0)
#define DISMCYCINT            DISMCYCINT_SHIFT(1)
