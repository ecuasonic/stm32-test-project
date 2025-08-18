#ifndef SCS_INTERRUPT_TYPE_H
#define SCS_INTERRUPT_TYPE_H

#include "types.h"

// Interrupt Type Register
struct ITM_Type {
    // ==========================================
    // Interrupt Controller Type Register (ICTR) (r)
    //    Reset value:
    //        Depends on number of interrupts defined in processor implementation.
    //    Bits:
    //        Interrupt Lines Number (INTLINESUM)
#define ICTR_INTLINESNUM(n) (((n)&0x1F) << 0)
    uint32_t ICTR;

    // ==========================================
    // Auxillary Control Register (ACTLR) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        Disbale IT Folding (DISFOLD)
#define ITM_DISFOLD     (1U << 2)
    //        Disable write buffer during default memory map accesses (DISDEFWBUF)
#define ITM_DISDEFWBUF  (1U << 1)
    //        Disable interruption of multi-cycle instructions (DISMCYCINT)
#define ITM_DISMCYCINT  (1U << 0)
    uint32_t ACTLR;
};

#define ITM ((struct ITM_Type *)0xE000E004UL)

#endif // SCS_INTERRUPT_TYPE_H