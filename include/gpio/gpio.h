#include "types.h"

#ifndef GPIO_H
#define GPIO_H

struct gpio {
    volatile uint32_t CRL;  // Port Configuration Register Low
    volatile uint32_t CRH;  // Port Configuration Register High
    volatile uint32_t IDR;  // Port Input Data
    volatile uint32_t ODR;  // Port Output Data
    volatile uint32_t BSRR; // Port Bit Set/Reset Register
    volatile uint32_t BRR;  // Port Bit Reset Register
    volatile uint32_t LCKR; // Port Configuration Lock Register
};

enum {
  GPIO_MODE_INPUT,
  GPIO_MODE_OUTPUT_10MHZ,
  GPIO_MODE_OUTPUT_2MHZ,
  GPIO_MODE_OUTPUT_50MHZ
};

#define PERIPH_BASE       0x40000000
#define APB2PERIPH_BASE   (PERIPH_BASE + 0x10000)
#define GPIO_BASE         (APB2PERIPH_BASE + 0x800)

#define GPIO(bank)        ((struct gpio *)(GPIO_BASE + 0x400 * ((bank) - 'A')))
#define PIN(bank, num)    ((((bank) - 'A') << 8) | (num)) // Encodes bank + pinnum in same int
#define PINNO(pin)        ((pin) & 255)
#define PINBANK(pin)      (((pin) >> 8) + 'A')

#endif
