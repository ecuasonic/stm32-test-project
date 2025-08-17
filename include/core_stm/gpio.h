#ifndef GPIO_H
#define GPIO_H

#include "types.h"

struct gpio {
    // ==========================================
    // Port Configuration Register Low (GPIOx_CRL) (x=A..G):
    //    Reset value:
    //        0x4444_4444
    //    Bits:
    //        CNFy[1:0] (Port x configuration bit (y=0..7)) (rw)
#define GPIO_CRL_CNF(y, n) (((n)&0x3U) << ((((y)&0x7U)*4))+2)
    //        MODEy[1:0] (Port x mode bit (y=0..7)) (rw)
#define GPIO_CRL_MODE(y, n) (((n)&0x3U) << (((y)&0x7U)*4))
    uint32_t CRL;

    // ==========================================
    // Port Configuration Register High (GPIOx_CRH) (x=A..G):
    //    Reset value:
    //        0x4444_4444
    //    Bits:
    //        CNFy[1:0] (Port x configuration bit (y=8..15)) (rw)
#define GPIO_CRH_CNF(pin, cnf)  ((uint32_t)(cnf) << (((pin)&0x7U)*4+2))
    //        MODEy[1:0] (Port x mode bit (y=8..15)) (rw)
#define GPIO_CRH_MODE(pin, mode) ((uint32_t)(mode) << (((pin)&0x7U)*4))
    uint32_t CRH;

    // ==========================================
    // Port Input Data Register (GPIOx_IDR) (x=A..G):
    //    Reset value:
    //        0x0000_XXXX
    //    Bits:
    //        IDRy (Port input data (y=0..15)) (r)
#define GPIO_IDR(n) ((n)&0xFFFFU)
    uint32_t IDR;

    // ==========================================
    // Port Output Data Register (GPIOx_ODR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        ODRy (Port output data (y=0..15)) (rw)
#define GPIO_ODR(n) ((n)&0xFFFFU)
    uint32_t ODR;

    // ==========================================
    // Port Bit Set/Reset Register (GPIOx_BSRR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        BRy (Port x Reset bit y (y=0..15)) (w)
#define GPIO_BSRR_BR(n) ((n)&0xFFFFU)
    //        BSy (Port x Set bit y (y=0..15)) (w)
#define GPIO_BSRR_BS(n) (((n)&0xFFFFU) << 16)
    uint32_t BSRR;

    // ==========================================
    // Port Bit Reset Register (GPIOx_BRR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        BRy (Port x Reset bit y (y=0..15)) (w)
#define GPIO_BRR_BR(n) ((n)&0xFFFFU)
    uint32_t BRR;

    // ==========================================
    // Port Configuration Lock Register (GPIOx_LCKR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        LCKK (Lock key) (rw)
#define GPIO_LCKR_LCKK    (1U << 16)
    //        LCKy (Port x Lock bit y (y=0..15)) (rw)
#define GPIO_LCKR_LCK(n)    (((n)&0x1U) << 16)
    uint32_t LCKR;
};

#define GPIO(bank)            ((struct gpio *)(0x40010800 + (0x0400 * (bank - 'A'))))
#define ENC_PIN(bank, num)    ((((bank) - 'A') << 8) | (num)) // Encodes bank + pinnum in same int
#define DEC_PINNO(pin)        ((pin) & 0xFF)
#define DEC_PINBANK(pin)      (((pin) >> 8) + 'A')

// ======================================================================================================

INLINE void
gpio_set(uint32_t bank, uint32_t pin) {
    struct gpio *gpio = GPIO(bank);
    gpio->BSRR = 1 << pin;
}

INLINE void
gpio_clear(uint32_t bank, uint32_t pin) {
    struct gpio *gpio = GPIO(bank);
    gpio->BSRR = 1 << (pin + 16);
}

#endif
