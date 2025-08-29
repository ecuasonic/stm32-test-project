#ifndef SYSTICK_H
#define SYSTICK_H

#include "types.h"

// System Timer
struct systick {
    // ==========================================
    // Control and Status Register
    //    Reset value: 0x0000_0000
    //    Bits:
    //        COUNTFLAG - Set when timer reaches 0 (cleared on read)
#define SYSTICK_CSR_COUNTFLAG   (1U << 16)
    //        CLKSOURCE - 0: External reference, 1: Processor clock
#define SYSTICK_CSR_CLKSOURCE   (1U << 2)
    //        TICKINT - 1: SysTick exception request on COUNTFLAG
#define SYSTICK_CSR_TICKINT     (1U << 1)
    //        ENABLE - Counter enable
#define SYSTICK_CSR_ENABLE      (1U << 0)
    uint32_t CSR;

    // ==========================================
    // Reload Value Register (r/w) (0)
    //    Reset State:
    //        N/A
    //    Holds the value to reload into CVR when it reaches 0
#define SYSTICK_RVR_LOAD(n)     ((n)&0x00FFFFFFU)
    uint32_t RVR;

    // ==========================================
    // Current Value Register (r/w) (0)
    //    Reset State:
    //        N/A
    //    Contains the current timer value (write clears to 0)
#define SYSTICK_CVR_CURRENT(n) ((n)&0x00FFFFFFU)
    uint32_t CVR;

    // ==========================================
    // Calibration Value Register (r) (0)
    //    Reset State:
    //        CALIB
    //    Provides reference value for 10ms at core clock
    //    Bits:
    //        No Reference Clock Flag (r)
#define SYSTICK_CALIB_NOREF     (1U << 31)
    //        Skew Status Flag (r)
#define SYSTICK_CALIB_SKEW      (1U << 30)
    //        Reload Value for 10ms timing (r)
#define SYSTICK_CALIB_TENMS(n)  ((x)&0x00FFFFFF)
    uint32_t CALIB;
};

#define SYSTICK     ((struct systick *)0xE000E010UL)

INLINE void systick_init(uint32_t ticks) {
    SYSTICK->CSR = SYSTICK_CSR_ENABLE | SYSTICK_CSR_TICKINT | SYSTICK_CSR_CLKSOURCE;
    SYSTICK->RVR = SYSTICK_RVR_LOAD(ticks - 1);
    SYSTICK->CVR = SYSTICK_CVR_CURRENT(0);
}

extern volatile uint32_t s_ticks;
uint32_t timer_expired(uint32_t *t, uint32_t prd, uint32_t now);
void delay(uint32_t ms);

#endif // SYSTICK_H