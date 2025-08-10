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

#endif
