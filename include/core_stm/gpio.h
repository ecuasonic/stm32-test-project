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
#define GPIO_CNF(pin, cnf)      (((cnf)&0x3U) << (((pin)%8U)*4+2))
    //        MODEy[1:0] (Port x mode bit (y=0..7)) (rw)
#define GPIO_MODE(pin, mode)    (((mode)&0x3U) << (((pin)%8U)*4))
    uint32_t CRL;

    // ==========================================
    // Port Configuration Register High (GPIOx_CRH) (x=A..G):
    //    Reset value:
    //        0x4444_4444
    //    Bits:
    //        CNFy[1:0] (Port x configuration bit (y=8..15)) (rw)
    //        MODEy[1:0] (Port x mode bit (y=8..15)) (rw)
    uint32_t CRH;

    // ==========================================
    // Port Input Data Register (GPIOx_IDR) (x=A..G):
    //    Reset value:
    //        0x0000_XXXX
    //    Bits:
    //        IDRy (Port input data (y=0..15)) (r)
#define GPIO_IDR(n) (1 << ((n)&0xFU))
    uint32_t IDR;

    // ==========================================
    // Port Output Data Register (GPIOx_ODR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        ODRy (Port output data (y=0..15)) (rw)
#define GPIO_ODR(n) (1 << ((n)&0xFU))
    uint32_t ODR;

    // ==========================================
    // Port Bit Set/Reset Register (GPIOx_BSRR) (x=A..G):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        BRy (Port x Reset bit y (y=0..15)) (w)
#define GPIO_BSRR_BR(n) (((n)&0xFFFFU) << 16)
    //        BSy (Port x Set bit y (y=0..15)) (w)
#define GPIO_BSRR_BS(n) ((n)&0xFFFFU)
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

// =============================================================================

// PU/PD are 0x2, but use bit[3] as differentiator
enum GPIO_CNF_IN {
    IN_ANALOG = 0x0,
    IN_FLOAT,
    IN_PD = 0x2,
    IN_PU = 0x2 | (1 << 3),
};

enum GPIO_CNF_OUT {
    OUT_PP = 0x0,
    OUT_OD,
    OUT_AF_PP,
    OUT_AF_OD
};

enum GPIO_MODE {
    IN = 0x0,
    OUT_10MHZ,
    OUT_2MHZ,
    OUT_50MHZ
};

void gpio_set(uint32_t bank, uint32_t pin);
void gpio_clear(uint32_t bank, uint32_t pin);
void config_gpio(uint32_t bank, uint32_t pin, uint32_t mode, uint32_t cnf);

#endif
