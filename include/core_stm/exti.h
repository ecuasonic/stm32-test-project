#ifndef EXTI_H
#define EXTI_H

#include "types.h"

struct exti {
    // ==========================================
    // Interrupt mask register (EXTI_IMR) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        MRx (Interrupt Mask on line x)
#define EXTI_MR(x)    (1U << (x)) // up to 18
    uint32_t IMR;

    // ==========================================
    // Event mask register (EXTI_EMR) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        MRx (Interrupt Mask on line x)
#define EXTI_MR(x)    (1U << (x)) // up to 18
    uint32_t EMR;

    // ==========================================
    // Rising trigger selection register (EXTI_RTSR) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        TRx (Rising trigger event configuration bit on line x)
#define EXTI_TR(x)    (1U << (x)) // up to 18
    uint32_t RTSR;

    // ==========================================
    // Falling trigger selection register (EXTI_FTSR) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        TRx (Falling trigger event configuration bit on line x)
#define EXTI_TR(x)    (1U << (x)) // up to 18
    uint32_t FTSR;

    // ==========================================
    // Software interrupt event register (EXTI_SWIER) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        TRx (Software interrupt on line x)
#define EXTI_SWIER(x)    (1U << (x)) // up to 18
    uint32_t SWIER;

    // ==========================================
    // Pending register (EXTI_PR) (rc_w1)
    //    Reset value:
    //        Undefined
    //    Bits:
    //        PRx (Pending bit)
#define EXTI_PR(x)    (1U << (x)) // up to 18
    uint32_t PR;
};

#define EXTI ((struct exti *)0x40010400)

// ====================================================================

#define IMR_bit     (1 << 0)
#define EMR_bit     (1 << 1)
#define RTSR_bit    (1 << 2)
#define FTSR_bit    (1 << 3)
// Usage: config_intr(0, 'B', IMR_bit | EMR_bit | RTSR_bit | FTSR_bit);
void config_exti(uint32_t exti, uint32_t port, uint32_t conf);
__attribute__((noreturn)) void panic(void);

#endif
