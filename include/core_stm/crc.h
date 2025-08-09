#ifndef CRC_H
#define CRC_H

#include "types.h"

struct crc {
    // ==========================================
    // Data Register (CRC_DR)
    //    Reset value:
    //        0xFFFF_FFFF
    volatile uint32_t DR; // (rw) (0)

    // ==========================================
    // Independent Data Register (CRC_IDR)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        General-purpose 8-bit data register bits (rw) (0)
#define CRC_IDR(n)    ((n)&0xFF)
    volatile uint32_t IDR;

    // ==========================================
    // Control Register (CRC_CR)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        RESET bit (w) (0)
#define CRC_CR_RESET(n)   (1 << 0)
    volatile uint32_t CR;
};

#define CRC ((struct crc *)0x40023000)

#endif
