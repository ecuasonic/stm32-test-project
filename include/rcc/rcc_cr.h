// Clock Control Register (RCC_CR)
//
// Address offset: 0x00
// Reset Value: 0x0000_XX83, where X is undefined.
// Access: no wait state, word, half-word, and byte access.

// ==================== PLL3 ===========================

// PLL3 Clock Ready Flag.
#define RCC_CR_PLL3RDY_SHIFT(n) ((n) << 29)
#define RCC_CR_PLL3RDY_MASK     RCC_CR_PLL3RDY_SHIFT(0x1)
#define RCC_CR_PLL3RDY          RCC_CR_PLL3RDY_SHIFT(0x1)

// PLL3 Enable.
#define RCC_CR_PLL3ON_SHIFT(n)  ((n) << 28)
#define RCC_CR_PLL3ON_MASK      RCC_CR_PLL3ON_SHIFT(0x1)
#define RCC_CR_PLL3ON           RCC_CR_PLL3ON_SHIFT(0x1)

// ==================== PLL2 ===========================

// PLL2 Clock Ready Flag.
#define RCC_CR_PLL2RDY_SHIFT(n) ((n) << 27)
#define RCC_CR_PLL2RDY_MASK     RCC_CR_PLL2RDY_SHIFT(0x1)
#define RCC_CR_PLL2RDY          RCC_CR_PLL2RDY_SHIFT(0x1)

// PLL2 Enable.
#define RCC_CR_PLL2ON_SHIFT(n)  ((n) << 26)
#define RCC_CR_PLL2ON_MASK      RCC_CR_PLL2ON_SHIFT(0x1)
#define RCC_CR_PLL2ON           RCC_CR_PLL2ON_SHIFT(0x1)

// ==================== PLL ============================

// PLL Clock Ready Flag.
#define RCC_CR_PLLRDY_SHIFT(n)  ((n) << 25)
#define RCC_CR_PLLRDY_MASK      RCC_CR_PLLRDY_SHIFT(0x1)
#define RCC_CR_PLLRDY           RCC_CR_PLLRDY_SHIFT(0x1)

// PLL Enable.
#define RCC_CR_PLLON_SHIFT(n)   ((n) << 24)
#define RCC_CR_PLLON_MASK       RCC_CR_PLLON_SHIFT(0x1)
#define RCC_CR_PLLON            RCC_CR_PLLON_SHIFT(0x1)

// ==================== HSE ============================

// CSSON: Clock Security System Enable.
// HSE Clock Detector.
#define RCC_CR_CSSON_SHIFT(n)   ((n) << 19)
#define RCC_CR_CSSON_MASK       RCC_CR_CSSON_SHIFT(0x1)
#define RCC_CR_CSSON            RCC_CR_CSSON_SHIFT(0x1)

// HSEBYP: External High-Speed Clock Bypass.
#define RCC_CR_HSEBYP_SHIFT(n)  ((n) << 18)
#define RCC_CR_HSEBYP_MASK      RCC_CR_HSEBYP_SHIFT(0x1)
#define RCC_CR_HSEBYP           RCC_CR_HSEBYP_SHIFT(0x1)

// HSERDY: External High-Speed Clock Ready Flag.
#define RCC_CR_HSERDY_SHIFT(n)  ((n) << 17)
#define RCC_CR_HSERDY_MASK      RCC_CR_HSERDY_SHIFT(0x1)
#define RCC_CR_HSERDY           RCC_CR_HSERDY_SHIFT(0x1)

// HSEON: External High-Speed Clock Enable.
#define RCC_CR_HSEON_SHIFT(n)   ((n) << 16)
#define RCC_CR_HSEON_MASK       RCC_CR_HSEON_SHIFT(0x1)
#define RCC_CR_HSEON            RCC_CR_HSEON_SHIFT(0x1)

// ==================== HSI ============================

// HSICAL: Internal High-Speed Clock Calibration.
#define RCC_CR_HSICAL_SHIFT(n)  ((n) << 8)
#define RCC_CR_HSICAL_MASK      RCC_CR_HSICAL_SHIFT(0xFF)

// HSITRIM: Internal High-Speed Clock Trimming.
// Added to HSICAL[7:0] bits.
// Default: 16
#define RCC_CR_HSITRIM_SHIFT(n) ((n) << 3)
#define RCC_CR_HSITRIM_MASK     RCC_CR_HSITRIM_SHIFT(0x1F)
#define RCC_CR_HSITRIM(n)       RCC_CR_HSITRIM_SHIFT(n & 0x1F)

// HSIRDY: Internal High-Speed Clock Ready Flag.
#define RCC_CR_HSIRDY_SHIFT(n)  ((n) << 1)
#define RCC_CR_HSIRDY           RCC_CR_HSIRDY_SHIFT(0x1)

// HSION: Internal High-Speed Clock Enable.
#define RCC_CR_HSION_SHIFT(n)   ((n) << 0)
#define RCC_CR_HSION_MASK       RCC_CR_HSION_SHIFT(0x1)
#define RCC_CR_HSION            RCC_CR_HSION_SHIFT(0x1)

/*
 * HSI (High-Speed Internal Osciallator) (On-Chip)
 *    Internal RC osciallator.
 *    Typically 8 MHz to 16 MHz.
 *    Accuracy: ~1% - good enough for many purposes but not precise for USB, audio, etc.
 *    Fast, startup, no external components required.
 *    Can be used to feed PLL.
 *
 * HSE (High-Speed External Osciallator) (On-Board)
 *    External crystal/resonator or oscillator.
 *    Typically 4 MHz to 25 MHz.
 *    Much more accurate and stable than HSI.
 *    Often used when precise timing is needed (USB, CAN, Ethernet, etc.).
 *    Can also be used to feed PLL.
 *
 * PLL (Phase-Locked Loop)
 *    Takes input from HSE or HSI.
 *    Typically 16 MHz to 72+ MHz.
 *    Multiplies the input frequency to reach a higher system clock (SYSCLK).
 *    Used to generate the main system frequency and derived clocks
 *      (USB, ADC, etc.).
 *    SYSCLK = PLLCLK = (HSE or HSI / prediv) x multiplier.
*/
