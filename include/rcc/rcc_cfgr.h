// Microcontroller Clock Output
#define RCC_CFGR_MCO_SHIFT(n)       ((n) << 24)
#define RCC_CFGR_MCO_MASK           RCC_CFGR_MCO_SHIFT(0xF)
#define RCC_CFGR_MCO_NOCLK          RCC_CFGR_MCO_SHIFT(0x0)
#define RCC_CFGR_MCO_SYSCLK         RCC_CFGR_MCO_SHIFT(0x4)
#define RCC_CFGR_MCO_HSICLK         RCC_CFGR_MCO_SHIFT(0x5)
#define RCC_CFGR_MCO_HSECLK         RCC_CFGR_MCO_SHIFT(0x6)
#define RCC_CFGR_MCO_PLLDIV2CLK     RCC_CFGR_MCO_SHIFT(0x7)
#define RCC_CFGR_MCO_PLL2           RCC_CFGR_MCO_SHIFT(0x8)
#define RCC_CFGR_MCO_PLL3DIV2CLK    RCC_CFGR_MCO_SHIFT(0x9)
#define RCC_CFGR_MCO_XT1            RCC_CFGR_MCO_SHIFT(0xA)
#define RCC_CFGR_MCO_PLL3           RCC_CFGR_MCO_SHIFT(0xB)

// ==================== Prescalers ========================

// ADC Prescaler.
#define RCC_CFGR_ADCPRE_SHIFT(n)    ((n) << 14)
#define RCC_CFGR_ADCPRE_MASK        RCC_CFGR_ADCPRE_SHIFT(0x3)
#define RCC_CFGR_ADCPRE_DIV2        RCC_CFGR_ADCPRE_SHIFT(0x0)
#define RCC_CFGR_ADCPRE_DIV4        RCC_CFGR_ADCPRE_SHIFT(0x1)
#define RCC_CFGR_ADCPRE_DIV6        RCC_CFGR_ADCPRE_SHIFT(0x2)
#define RCC_CFGR_ADCPRE_DIV8        RCC_CFGR_ADCPRE_SHIFT(0x3)

// APB High-Speed Prescaler (APB2) (DIV on HCLK) (PCLK2)
#define RCC_CFGR_PPRE2_SHIFT(n)     ((n) << 11)
#define RCC_CFGR_PPRE2_MASK         RCC_CFGR_PPRE2_SHIFT(0x7)
#define RCC_CFGR_PPRE2_NODIV        RCC_CFGR_PPRE2_SHIFT(0x0)
#define RCC_CFGR_PPRE2_DIV2         RCC_CFGR_PPRE2_SHIFT(0x4)
#define RCC_CFGR_PPRE2_DIV4         RCC_CFGR_PPRE2_SHIFT(0x5)
#define RCC_CFGR_PPRE2_DIV8         RCC_CFGR_PPRE2_SHIFT(0x6)
#define RCC_CFGR_PPRE2_DIV16        RCC_CFGR_PPRE2_SHIFT(0x7)

// APB Low-Speed Prescaler (APB1) (DIV on HCLK) (PCLK1)
#define RCC_CFGR_PPRE1_SHIFT(n)     ((n) << 8)
#define RCC_CFGR_PPRE1_MASK         RCC_CFGR_PPRE1_SHIFT(0x7)
#define RCC_CFGR_PPRE1_NODIV        RCC_CFGR_PPRE1_SHIFT(0x0)
#define RCC_CFGR_PPRE1_DIV2         RCC_CFGR_PPRE1_SHIFT(0x4)
#define RCC_CFGR_PPRE1_DIV4         RCC_CFGR_PPRE1_SHIFT(0x5)
#define RCC_CFGR_PPRE1_DIV8         RCC_CFGR_PPRE1_SHIFT(0x6)
#define RCC_CFGR_PPRE1_DIV16        RCC_CFGR_PPRE1_SHIFT(0x7)

// AHB Prescaler (DIV on SYSCLK) (AHB)
#define RCC_CFGR_HPRE_SHIFT(n)      ((n) << 4)
#define RCC_CFGR_HPRE_MASK          RCC_CFGR_HPRE_SHIFT(0xF)
#define RCC_CFGR_HPRE_NODIV         RCC_CFGR_HPRE_SHIFT(0x0)
#define RCC_CFGR_HPRE_DIV2          RCC_CFGR_HPRE_SHIFT(0x8)
#define RCC_CFGR_HPRE_DIV4          RCC_CFGR_HPRE_SHIFT(0x9)
#define RCC_CFGR_HPRE_DIV8          RCC_CFGR_HPRE_SHIFT(0xA)
#define RCC_CFGR_HPRE_DIV16         RCC_CFGR_HPRE_SHIFT(0xB)
#define RCC_CFGR_HPRE_DIV64         RCC_CFGR_HPRE_SHIFT(0xC)
#define RCC_CFGR_HPRE_DIV128        RCC_CFGR_HPRE_SHIFT(0xD)
#define RCC_CFGR_HPRE_DIV256        RCC_CFGR_HPRE_SHIFT(0xE)
#define RCC_CFGR_HPRE_DIV512        RCC_CFGR_HPRE_SHIFT(0xF)

// ================== SYSCLK Source ======================

// System Clock Switch Status
// To check SYSCLK source.
#define RCC_CFGR_SWS_SHIFT(n)       ((n) << 2)
#define RCC_CFGR_SWS_MASK           RCC_CFGR_SWS_SHIFT(0x3)
#define RCC_CFGR_SWS_HSI            RCC_CFGR_SWS_SHIFT(0x0)
#define RCC_CFGR_SWS_HSE            RCC_CFGR_SWS_SHIFT(0x1)
#define RCC_CFGR_SWS_PLL            RCC_CFGR_SWS_SHIFT(0x2)

// System Clock Switch
// To select the SYSCLK source.
#define RCC_CFGR_SW_SHIFT(n)        ((n) << 0)
#define RCC_CFGR_SW_MASK            RCC_CFGR_SW_SHIFT(0x3)
#define RCC_CFGR_SW_HSI             RCC_CFGR_SW_SHIFT(0x0)
#define RCC_CFGR_SW_HSE             RCC_CFGR_SW_SHIFT(0x1)
#define RCC_CFGR_SW_PLL             RCC_CFGR_SW_SHIFT(0x2)
