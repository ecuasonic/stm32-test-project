#include "types.h"
#include "rcc/rcc_cfgr.h"
#include "rcc/rcc_cr.h"

#ifndef RCC_H
#define RCC_H

struct rcc {
  volatile uint32_t CR;         // Clock Control Register
  volatile uint32_t CFGR;       // Clock Configuration Register
  volatile uint32_t CIR;        // Clock Interrupt Register
  volatile uint32_t APB2RSTR;   // APB2 Peripheral Reset Register
  volatile uint32_t APB1RSTR;   // APB1 Peripheral Reset Register
  volatile uint32_t AHBENR;     // AHB Peripheral Reset Register
  volatile uint32_t APB2ENR;    // APB2 Peripheral Clock Enable Register
  volatile uint32_t APB1ENR;    // APB1 Peripheral Clock Enable Register
  volatile uint32_t BDCR;       // Backup Domain Control Register
  volatile uint32_t CSR;        // Control/Status Register
  volatile uint32_t AHBRSTR;    // AHB Peripheral Clock Reset Register
  volatile uint32_t CFGR2;      // Clock Configuration Register 2
};

#define RCC_BASE              0x40021000
#define RCC                   ((struct rcc *)(RCC_BASE))

#define RCC_IOPAEN            (1 << 2)
#define RCC_IOPBEN            (1 << 3)
#define RCC_IOPCEN            (1 << 4)

#endif
