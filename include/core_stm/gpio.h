#include "types.h"

#ifndef GPIO_H
#define GPIO_H

struct GPIO {
    // ==========================================
    // Port Configuration Register Low (GPIOx_CRL) (x=A..G):
    //    Reset value:
    //        0x4444_4444
    //    Bits:
    //        CNFy[1:0] (Port x configuration bit (y=0..7)) (rw)
#define GPIO_CRL_CNF(y, n) (((n)&0x3) << (((y)&0x7)*4)+2)
    //        MODEy[1:0] (Port x mode bit (y=0..7)) (rw)
#define GPIO_CRL_MODE(y, n) (((n)&0x3) << (((y)&0x7)*4))
    uint32_t CRL;

    // ==========================================
    // Port Configuration Register High (GPIOx_CRH) (x=A..G):
    //    Reset value:
    //        0x4444_4444
    //    Bits:
    //        CNFy[1:0] (Port x configuration bit (y=8..15)) (rw)
#define GPIO_CRH_CNF(y, n) (((n)&0x3) << (((y)&0x7)*4)+2)
    //        MODEy[1:0] (Port x mode bit (y=8..15)) (rw)
#define GPIO_CRH_MODE(y, n) (((n)&0x3) << (((y)&0x7)*4))
    uint32_t CRH;

    // ==========================================
    // Port Input Data Register (GPIOx_IDR) (x=A..G):
    //    Reset value:
    //        0x0000_XXXX
    //    Bits:
    //        IDRy (Port input data (y=0..15)) (r)
#define GPIO_IDR(n) ((n)&0xFFFF)
    uint32_t IDR;

    // ==========================================
    // Port Output Data Register (GPIOx_ODR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        ODRy (Port output data (y=0..15)) (rw)
#define GPIO_ODR(n) ((n)&0xFFFF)
    uint32_t ODR;

    // ==========================================
    // Port Bit Set/Reset Register (GPIOx_BSRR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        BRy (Port x Reset bit y (y=0..15)) (w)
#define GPIO_BSRR_BR(n) ((n)&0xFFFF)
    //        BSy (Port x Set bit y (y=0..15)) (w)
#define GPIO_BSRR_SR(n) (((n)&0xFFFF) << 16)
    uint32_t BSRR;

    // ==========================================
    // Port Bit Reset Register (GPIOx_BRR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        BRy (Port x Reset bit y (y=0..15)) (w)
#define GPIO_BRR_BR(n) ((n)&0xFFFF)
    uint32_t BRR;

    // ==========================================
    // Port Configuration Lock Register (GPIOx_LCKR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        LCKK (Lock key) (rw)
#define GPIO_LCKR_LCKK    (1 << 16)
    //        LCKy (Port x Lock bit y (y=0..15)) (rw)
#define GPIO_LCKR_LCK(n)    (((n)&0x1) << 16)
    uint32_t LCKR;
};

#define GPIO(bank)        ((struct GPIO *)(0x40010800 + (0x0400 * (bank - 'A'))))
#define PIN(bank, num)    ((((bank) - 'A') << 8) | (num)) // Encodes bank + pinnum in same int
#define PINNO(pin)        ((pin) & 255)
#define PINBANK(pin)      (((pin) >> 8) + 'A')

#endif
