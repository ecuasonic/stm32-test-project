#ifndef DEFINES_H
#define DEFINES_H

// =================================
//              RCC
// =================================
// SYSCLK sources:
//      HSI (8 MHz), HSE, PLL
// MCO sources:
//      SYSCLK, HSI, HSE, PLL/2 (Must be at most 50 MHz)
// PLL sources:
//      (HSI/2, HSE)*(2..16)
// -- MCO
#define CLEAR_MCO       ~RCC_CFGR_MCO(0x7)
#define SET_MCO(mco)    RCC_CFGR_MCO(mco)
#define SET_MCO_HSI     SET_MCO(0x5)
#define SET_MCO_PLL     SET_MCO(0x7)
// -- PLL
#define CLEAR_PLLMUL    ~RCC_CFGR_PLLMUL(0xF)
#define SET_HSI_HALF    ~RCC_CFGR_PLLSRC
#define SET_PLLMUL_2    RCC_CFGR_PLLMUL(0x0)
#define SET_PLLMUL_3    RCC_CFGR_PLLMUL(0x1)
#define SET_PLLMUL_4    RCC_CFGR_PLLMUL(0x2)
#define SET_PLLMUL_5    RCC_CFGR_PLLMUL(0x3)
#define SET_PLLMUL_8    RCC_CFGR_PLLMUL(0x6)
#define SET_PLLMUL_15   RCC_CFGR_PLLMUL(0xD)
#define SET_PLLMUL_16   RCC_CFGR_PLLMUL(0xF)
// -- HPRE (AHB PRE)
#define CLEAR_HPRE      ~RCC_CFGR_HPRE(0xF)
#define SET_HPRE_2      RCC_CFGR_HPRE(0x8)
#define SET_HPRE_4      RCC_CFGR_HPRE(0x9)
#define SET_HPRE_8      RCC_CFGR_HPRE(0xA)

// =================================
//              GPIO
// =================================
// -- CNF
#define CLEAR_CNF(pin)          (~GPIO_CNF(pin, 0x3))
#define SET_CNF(pin, cnf)       GPIO_CNF(pin, cnf)
#define SET_CNF_IN_ANALOG(pin)  SET_CNF(pin, 0x0)
#define SET_CNF_IN_FLOAT(pin)   SET_CNF(pin, 0x1)
#define SET_CNF_IN_PUPD(pin)    SET_CNF(pin, 0x2)
#define SET_CNF_OUT_PP(pin)     SET_CNF(pin, 0x0)
#define SET_CNF_OUT_OD(pin)     SET_CNF(pin, 0x1)
#define SET_CNF_OUT_AF_PP(pin)  SET_CNF(pin, 0x2)
#define SET_CNF_OUT_AF_OD(pin)  SET_CNF(pin, 0x3)
// -- MODE
#define SET_MODE(pin, mode)     GPIO_MODE(pin, mode)
#define CLEAR_MODE(pin)         (~GPIO_MODE(pin, 0x3))
#define SET_MODE_IN(pin)        SET_MODE(pin, 0x0)
#define SET_MODE_OUT_10MHZ(pin) SET_MODE(pin, 0x1)
#define SET_MODE_OUT_2MHZ(pin)  SET_MODE(pin, 0x2)
#define SET_MODE_OUT_50MHZ(pin) SET_MODE(pin, 0x3)
// -- CNF and MODE
#define CLEAR_PIN(pin)          CLEAR_CNF(pin) & CLEAR_MODE(pin)

// =================================
//              AFIO
// =================================
// --
#define CLEAR_EXTICR1   ~(AFIO_EXTICR1_EXTI(0, 0xF))

// =================================================

#endif