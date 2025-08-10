#ifndef RCC_H
#define RCC_H

#include "types.h"

struct rcc {
    // ==========================================
    // Clock Control Register (RCC_CR):
    //    Reset value:
    //        0x0000_XX83 where X is undefined.
    //    Access:
    //        No wait state, word, half-word and byte access.
    //    Bits:
    //        Ready flags (r)
#define RCC_CR_PLLRDY     (1U << 25)   // (0)
#define RCC_CR_HSERDY     (1U << 17)   // (0)
#define RCC_CR_HSIRDY     (1U << 1)    // (1)
                                      //        Enable clock (rw)
#define RCC_CR_PLLON      (1U << 24)   // (0)
#define RCC_CR_CSSON      (1U << 19)   // (0)
#define RCC_CR_HSEON      (1U << 16)   // (0)
#define RCC_CR_HSION      (1U << 0)    // (1)
    //        HSE Bypass (rw) (0)
#define RCC_CR_HSEBYP     (1U << 18)
    //        Calibration
#define RCC_CR_HSICAL(n)  (((n)&0xFFU) << 8)   // (r)  (X)
#define RCC_CR_HSITRIM(n) (((n)&0x1FU) << 3)   // (rw) (0x10)
    uint32_t CR;

    // ==========================================
    // Clock Configuration Register (RCC_CFGR):
    //    Reset value:
    //        0x0000_0000
    //    Access:
    //        0 <= wait state <= 2, word, half-word and byte access.
    //        1 or 2 wait states inserted only if the access occurs during clock source switch.
    //    Bits:
    //        MCO (Microcontroller clock output) (rw) (0)
#define RCC_CFGR_MCO(n)    (((n)&0x7U) << 24)
    //        Prescalar (rw) (0)
#define RCC_CFGR_USBPRE    (1U << 22)
#define RCC_CFGR_ADCPRE(n) (((n)&0x3U) << 14)
#define RCC_CFGR_PPRE2(n)  (((n)&0x7U) << 11)
#define RCC_CFGR_PPRE1(n)  (((n)&0x7U) << 8)
#define RCC_CFGR_HPRE(n)   (((n)&0xFU) << 4)
    //        PLL (0)
#define RCC_CFGR_PLLMUL(n) (((n)&0xFU) << 18)   // (r)
#define RCC_CFGR_PLLXTPRE  (1U << 17)           // (rw)
#define RCC_CFGR_PLLSRC    (1U << 16)           // (rw)
    //        System clock switch (0)
#define RCC_CFGR_SWS(n)    (((n)&0x3U) << 2)    // (r)
#define RCC_CFGR_SW(n)     (((n)&0x3U) << 0)    // (rw)
    uint32_t CFGR;

    // ==========================================
    // Clock Interrupt Register (RCC_CIR):
    //    Reset value:
    //        0x0000_0000
    //    Access:
    //        No wait state, word, half-world and byte access
    //    Bits:
    //        System/Ready Interrupt clear (w) (0)
#define RCC_CIR_CSSC      (1U << 23)
#define RCC_CIR_PLLRDYC   (1U << 20)
#define RCC_CIR_HSERDYC   (1U << 19)
#define RCC_CIR_HSIRDYC   (1U << 18)
#define RCC_CIR_LSERDYC   (1U << 17)
#define RCC_CIR_LSIRDYC   (1U << 16)
    //        Ready interrupt enables (rw) (0)
#define RCC_CIR_PLLRDYIE  (1U << 12)
#define RCC_CIR_HSERDYIE  (1U << 11)
#define RCC_CIR_HSIRDYIE  (1U << 10)
#define RCC_CIR_LSERDYIE  (1U << 9)
#define RCC_CIR_LSIRDYIE  (1U << 8)
    //        System/Ready Interrupt Flags (r) (0)
#define RCC_CIR_CSSF      (1U << 7)
#define RCC_CIR_PLLRDYF   (1U << 4)
#define RCC_CIR_HSERDYF   (1U << 3)
#define RCC_CIR_HSIRDYF   (1U << 2)
#define RCC_CIR_LSERDYF   (1U << 1)
#define RCC_CIR_LSIRDYF   (1U << 0)
    uint32_t CIR;

    // ==========================================
    // APB2 Peripheral Reset Register (RCC_APB2RSTR):
    //    Reset value:
    //        0x0000_0000
    //    Access:
    //        No wait state, word, half-world and byte access
    //    Bits:
    //        Timer Clock Reset (rw) (0)
#define RCC_APB2RSTR_TIM11RST  (1U << 21)
#define RCC_APB2RSTR_TIM10RST  (1U << 20)
#define RCC_APB2RSTR_TIM9RST   (1U << 19)
#define RCC_APB2RSTR_TIM8RST   (1U << 13)
#define RCC_APB2RSTR_TIM1RST   (1U << 11)
    //        Misc. Clock Reset (rw) (0)
#define RCC_APB2RSTR_ADC3RST   (1U << 15)
#define RCC_APB2RSTR_USART1RST (1U << 14)
#define RCC_APB2RSTR_SPI1RST   (1U << 12)
#define RCC_APB2RSTR_ADC2RST   (1U << 10)
#define RCC_APB2RSTR_ADC1RST   (1U << 9)
    //        GPIO Clock Reset  (rw) (0)
#define RCC_APB2RSTR_IOGRST    (1U << 8)
#define RCC_APB2RSTR_IOFRST    (1U << 7)
#define RCC_APB2RSTR_IOERST    (1U << 6)
#define RCC_APB2RSTR_IODRST    (1U << 5)
#define RCC_APB2RSTR_IOCRST    (1U << 4)
#define RCC_APB2RSTR_IOBRST    (1U << 3)
#define RCC_APB2RSTR_IOARST    (1U << 2)
#define RCC_APB2RSTR_AFIORST   (1U << 0)
    uint32_t APB2RSTR;

    // ==========================================
    // APB1 Peripheral Reset Register (RCC_APB1RSTR):
    //    Reset value:
    //        0x0000_0000
    //    Access:
    //        No wait state, word, half-world and byte access
    //    Bits:
    //        Timer Clock Reset (rw) (0)
#define RCC_APB1RSTR_TIM14RST  (1U << 8)
#define RCC_APB1RSTR_TIM13RST  (1U << 7)
#define RCC_APB1RSTR_TIM12RST  (1U << 6)
#define RCC_APB1RSTR_TIM7RST   (1U << 5)
#define RCC_APB1RSTR_TIM6RST   (1U << 4)
#define RCC_APB1RSTR_TIM5RST   (1U << 3)
#define RCC_APB1RSTR_TIM4RST   (1U << 2)
#define RCC_APB1RSTR_TIM3RST   (1U << 1)
#define RCC_APB1RSTR_TIM2RST   (1U << 0)
    //        Communication Clock Reset (rw) (0)
#define RCC_APB1RSTR_CANRST    (1U << 25)
#define RCC_APB1RSTR_USBRST    (1U << 23)
#define RCC_APB1RSTR_I2C2RST   (1U << 22)
#define RCC_APB1RSTR_I2C1RST   (1U << 21)
#define RCC_APB1RSTR_UART5RST  (1U << 20)
#define RCC_APB1RSTR_UART4RST  (1U << 19)
#define RCC_APB1RSTR_USART3RST (1U << 18)
#define RCC_APB1RSTR_USART2RST (1U << 17)
#define RCC_APB1RSTR_SPI3RST   (1U << 15)
#define RCC_APB1RSTR_SPI2RST   (1U << 14)
    //        Misc. Clock Reset (rw) (0)
#define RCC_APB1RSTR_DACRST    (1U << 29)
#define RCC_APB1RSTR_PWRRST    (1U << 28)
#define RCC_APB1RSTR_BKPRST    (1U << 27)
#define RCC_APB1RSTR_WWDGRST   (1U << 11)
    uint32_t APB1RSTR;

    // ==========================================
    // AHB Peripheral Clock Enable Register (RCC_AHBENR):
    //    Reset value:
    //        0x0000_0014
    //    Access:
    //        No wait state, word, half-world and byte access
    //        When peripheral clock is not active, may not be readable and return 0x0.
    //    Bits:
    //        Enable component (rw)
#define RCC_AHBENR_SDIOEN     (1U << 10)   // (0)
#define RCC_AHBENR_FSMCEN     (1U << 8)    // (0)
#define RCC_AHBENR_CRCEN      (1U << 6)    // (0)
#define RCC_AHBENR_FLITFEN    (1U << 4)    // (1)
#define RCC_AHBENR_SRAMEN     (1U << 2)    // (1)
#define RCC_AHBENR_DMA2EN     (1U << 1)    // (0)
#define RCC_AHBENR_DMA1EN     (1U << 0)    // (0)
    uint32_t AHBENR;

    // ==========================================
    // APB2 Peripheral Clock Enable Register (RCC_APB2ENR):
    //    Reset value:
    //        0x0000_0000
    //    Access:
    //        Word, half-word and byte access
    //        No wait states, except until access to APB2 peripheral is finished.
    //        When peripheral clock is not active, may not be readable and return 0x0.
    //    Bits:
    //        Timer Clock Enable (rw) (0)
#define RCC_APB2ENR_TIM11EN  (1U << 21)
#define RCC_APB2ENR_TIM10EN  (1U << 20)
#define RCC_APB2ENR_TIM9EN   (1U << 19)
#define RCC_APB2ENR_TIM8EN   (1U << 13)
#define RCC_APB2ENR_TIM1EN   (1U << 11)
    //        Misc. Clock Enable (rw) (0)
#define RCC_APB2ENR_ADC3EN   (1U << 15)
#define RCC_APB2ENR_USART1EN (1U << 14)
#define RCC_APB2ENR_SPI1EN   (1U << 12)
#define RCC_APB2ENR_ADC2EN   (1U << 10)
#define RCC_APB2ENR_ADC1EN   (1U << 9)
    //        GPIO Clock Enable  (rw) (0)
#define RCC_APB2ENR_IOPGEN    (1U << 8)
#define RCC_APB2ENR_IOPFEN    (1U << 7)
#define RCC_APB2ENR_IOPEEN    (1U << 6)
#define RCC_APB2ENR_IOPDEN    (1U << 5)
#define RCC_APB2ENR_IOPCEN    (1U << 4)
#define RCC_APB2ENR_IOPBEN    (1U << 3)
#define RCC_APB2ENR_IOPAEN    (1U << 2)
#define RCC_APB2ENR_AFIOEN    (1U << 0)
    uint32_t APB2ENR;

    // ==========================================
    // APB1 Peripheral Clock Enable Register (RCC_APB1ENR):
    //    Reset value:
    //        0x0000_0000
    //    Access:
    //        Word, half-word and byte access
    //        No wait states, except until access to APB2 peripheral is finished.
    //        When peripheral clock is not active, may not be readable and return 0x0.
    //    Bits:
    //        Timer Clock Enable (rw) (0)
#define RCC_APB1ENR_TIM14EN  (1U << 8)
#define RCC_APB1ENR_TIM13EN  (1U << 7)
#define RCC_APB1ENR_TIM12EN  (1U << 6)
#define RCC_APB1ENR_TIM7EN   (1U << 5)
#define RCC_APB1ENR_TIM6EN   (1U << 4)
#define RCC_APB1ENR_TIM5EN   (1U << 3)
#define RCC_APB1ENR_TIM4EN   (1U << 2)
#define RCC_APB1ENR_TIM3EN   (1U << 1)
#define RCC_APB1ENR_TIM2EN   (1U << 0)
    //        Communication Clock Enable (rw) (0)
#define RCC_APB1ENR_CANEN    (1U << 25)
#define RCC_APB1ENR_USBEN    (1U << 23)
#define RCC_APB1ENR_I2C2EN   (1U << 22)
#define RCC_APB1ENR_I2C1EN   (1U << 21)
#define RCC_APB1ENR_UART5EN  (1U << 20)
#define RCC_APB1ENR_UART4EN  (1U << 19)
#define RCC_APB1ENR_USART3EN (1U << 18)
#define RCC_APB1ENR_USART2EN (1U << 17)
#define RCC_APB1ENR_SPI3EN   (1U << 15)
#define RCC_APB1ENR_SPI2EN   (1U << 14)
    //        Misc. Clock Enable (rw) (0)
#define RCC_APB1ENR_DACEN    (1U << 29)
#define RCC_APB1ENR_PWREN    (1U << 28)
#define RCC_APB1ENR_BKPEN    (1U << 27)
#define RCC_APB1ENR_WWDGEN   (1U << 11)
    uint32_t APB1ENR;

    // ==========================================
    // Backup Domain Control Register (RCC_BDCR):
    //    Reset value:
    //        0x0000_0000, reset by Backup domain Reset
    //    Access:
    //        0 <= wait state <= 3, Word, half-word and byte access
    //        Wait states are inserted in case of sucessive accesses to this register.
    //    Note:
    //        LSEON, LSEBYP, RTCSEL, RTCEN bits are in Backup domain.
    //        Thus, after Reset:
    //            1. These bits are write-protected.
    //            2. PWR_CR.DBP has to be set before these bits can be modified.
    //        Any internal or external Reset will not have any effect on these bits.
    //    Bits:
    //        RTC (rw) (0)
#define RCC_BDCR_RTCEN        (1U << 15)
#define RCC_BDCR_RTCSEL(n)    (((n)&0x3U) << 8)
    //        Backup Reset (rw) (0)
#define RCC_BDCR_BDRST        (1U << 16)
    //        LSE (0)
#define RCC_BDCR_LSEBYP       (1U << 2)    // (rw)
#define RCC_BDCR_LSERDY       (1U << 1)    // (r)
#define RCC_BDCR_LSEON        (1U << 0)    // (rw)
    uint32_t BDCR;

    // ==========================================
    // Control/Status Register (RCC_CSR)
    //    Reset value:
    //        0x0C00_0000, reset by system Reset, except reset flags by power Reset only
    //    Access:
    //        0 <= wait state <= 3, word, half-word and byte access
    //        Wait states are inserted in case of sucessive accesses to this register.
    //    Bits:
    //        Reset Status Flags (rw)
#define RCC_CSR_LPWRRSTF  (1U << 31)   // (0)
#define RCC_CSR_WWDGRSTF  (1U << 30)   // (0)
#define RCC_CSR_IWDGRSTF  (1U << 29)   // (0)
#define RCC_CSR_SFTRSTF   (1U << 28)   // (0)
#define RCC_CSR_PORRSTF   (1U << 27)   // (1)
#define RCC_CSR_PIRRSTF   (1U << 26)   // (1)
#define RCC_CSR_RMVF      (1U << 24)   // (0)
    //        LSI bits (0)
#define RCC_CSR_LSIRDY    (1U << 1)    // (r)
#define RCC_CSR_LSION     (1U << 0)    // (rw)
    uint32_t CSR;
};

#define RCC ((struct rcc *)0x40021000)

#endif
