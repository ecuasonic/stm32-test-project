#include "types.h"

// ARM's SysTick interrupt.
// SysTick is a 24-bit hardware counter, and is part of ARM core,
// therefore it is documented by the ARM datasheet.
//
// SysTick has four registers:
//    CTRL - used to enable/disable systick.
//    LOAD - an initial counter value, decremented on each clock cycle.
//    VAL - current counter value, decremented on each clock cycle.
//    CALIB - calibration register.
//
// Every time VAL drops to zero, a SysTick interrupt is generated.
// SysTick interrupt index in the vector table is 15.

struct systick {
  volatile uint32_t CTRL;   // SysTick Control and Status Register
  volatile uint32_t LOAD;   // SysTick Reload Value Register
  volatile uint32_t CURRENT;    // SysTick Curent Value Register
  volatile uint32_t CALIB;  // SysTick Calibration Value Register
};
#define NVIC_BASE   0xE000E000
#define SYSTICK     ((struct systick *)(NVIC_BASE + 0x10))

// ============= SysTick Control and Status Register ===============
// Access: Read/Write
// Reset state: 0x0000_0000
//
// Use to enable SysTick features.

#define COUNTFLAG_SHIFT(n)  ((n) << 16)
#define SYSTICK_COUNTFLAG   COUNTFLAG_SHIFT(0x1)

#define CLKSOURCE_SHIFT(n)  ((n) << 2)
#define SYSTICK_CLKSOURCE   CLKSOURCE_SHIFT(0x1)

#define TICKINT_SHIFT(n)    ((n) << 1)
#define SYSTICK_TICKINT     TICKINT_SHIFT(0x1)

#define ENABLE_SHIFT(n)     ((n) << 0)
#define SYSTICK_ENABLE      ENABLE_SHIFT(0x1)

// ============ SysTick Reload Value Register ===================
// Access: Read/Write
// Reset state: N/A
//
// Specify start value to load into current value register when counter reaches 0.
// 0 (no effect), 1 - 0x00FF_FFFF
// Mult-shot (set once):
//    Fires every N+1 clock pulses.
//    If tick interrupt required every 100 clock pulses, then LOAD=99.
// Single-shot (set every tickint):
//    Fires every N clock pulses.
//    If tick is next required after 400 clock pulses, then LOAD=400.

#define LOAD_SHIFT(n)       ((n) << 0)
#define SYSTICK_LOAD_MASK   0x00FFFFFF
#define SYSTICK_LOAD(n)     LOAD_SHIFT(n)

// =============== SysTick Current Value Register ===============
// Access: Read/Write clear
// Reset state: N/A
//
// Use to find the current value in the register.

#define CURRENT_SHIFT(n)          ((n) << 0)
#define SYSTICK_CURRENT_MASK      0x00FFFFFF
#define SYSTICK_CURRENT(n)        CURRENT_SHIFT(n)

// ============ SysTick Calibration Value Register ==============
// Access: Read
// Reset state: STCALIB
//
// Use to enable software to scale to any required speed using divide and multiply

#define NOREF_SHIFT(n)            ((n) << 31)
#define SYSTICK_NOREF_MASK        NOREF_SHIFT(0x1)
#define SYSTICK_NOREF             NOREF_SHIFT(0x1)

#define SKEW_SHIFT(n)     ((n) << 30)
#define SYSTICK_SKEW_MASK         SKEW_SHIFT(0x1)
#define SYSTICK_SKEW              SKEW_SHIFT(0x1)

#define TENMS_SHIFT       ((n) << 0)
#define SYSTICK_TENMS_MASK        0xFFFFFF
#define SYSTICK_TENMS(n)          TENMS_SHIFT(n & TENMS_MASK)
