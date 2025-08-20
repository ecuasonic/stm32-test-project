#ifndef NVIC_H
#define NVIC_H

#include "types.h"

struct NVIC {
    // ==========================================
    // Interrupt Set Enable (ISER) (r/w)
    //    Reset value: 
    //        0x0000_0000
#define NVIC_ISER_SETENA(n)     (1 << ((n)&0x1F))
    uint32_t ISER[8];
    uint32_t RESERVED0[24];

    // ==========================================
    // Interrupt Clear Enable (ICER) (r/w)
    //    Reset value: 
    //        0x0000_0000
#define NVIC_ICER_CLRENA(n)     ((n)&0xFFFFFFFF)
    uint32_t ICER[8];
    uint32_t RESERVED1[24];

    // ==========================================
    // Interrupt Set Pending (ISPR) (r/w)
    //    Reset value: 
    //        0x0000_0000
#define NVIC_ISPR_SETENA(n)     ((n)&0xFFFFFFFF)
    uint32_t ISPR[8];
    uint32_t RESERVED2[24];

    // ==========================================
    // Interrupt Clear Pending (ICPR) (r/w)
    //    Reset value: 
    //        0x0000_0000
#define NVIC_ICPR_CLRENA(n)     ((n)&0xFFFFFFFF)
    uint32_t ICPR[8];
    uint32_t RESERVED3[24];

    // ==========================================
    // Interrupt Active Bit (IABR) (r)
    //    Reset value: 
    //        0x0000_0000
#define NVIC_IABR_ACTIVE(n)     ((n)&0xFFFFFFFF)
    uint32_t IABR[8];
    uint32_t RESERVED4[56];

    // ==========================================
    // Interrupt Priority (IPR) (r/w)
    //    Reset value: 
    //        0x0000_0000
    uint32_t IPR[8];
};

#define NVIC ((struct NVIC *)0xE000E100UL)

#endif // NVIC_H