#ifndef AFIO_H
#define AFIO_H

#include "types.h"

struct mapr {
    uint32_t A;
    uint32_t B;
};

// To read/write AFIO_EVCR, AFIO_MAPR, and AFIO_EXTICRX registers, AFIO clock must be on.
struct afio {
    // ==========================================
    // Event control register (AFIO_EVCR) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        EVOE (Event output enable)
#define AFIO_EVOE     (1U << 7)
    //        PORT[2:0] (Port selection)
#define AFIO_PORT(n)  (((n)&0x7U) << 4)
    //        PIN[3:0] (Pin selection (x=A..E))
#define AFIO_PIN(n)   (((n)&0xFU) << 0)
    uint32_t EVCR;

    // ==========================================
    // AF remap and debug I/O configuration register (AFIO_EVCR) (rw)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        SWJ_CFG[2:0] (Serial wire JTAG configuration) (w) (0)
#define AFIO_MAPR_CFG(n)  (((n)&0x7u) << 24)
    //        ADC Remapping: (0)
    //            ADC2_ETRGREG_REMAP (ADC 2 external trigger regular conversion remapping)
#define AFIO_MAPR_ADC2_ETRGREG_REMAP  (1U << 20)
    //            ADC2_ETRGINJ_REMAP (ADC 2 external trigger injected conversion remapping)
#define AFIO_MAPR_ADC2_ETRGINJ_REMAP  (1U << 19)
    //            ADC1_ETRGREG_REMAP (ADC 1 external trigger regular conversion remapping)
#define AFIO_MAPR_ADC1_ETRGREG_REMAP  (1U << 18)
    //            ADC1_ETRGINJ_REMAP (ADC 1 external trigger injected conversion remapping)
#define AFIO_MAPR_ADC1_ETRGINJ_REMAP  (1U << 17)
    //        Timer Remapping: (0)
#define AFIO_MAPR_TIM5CH4_IREMAP      (1U << 16)
#define AFIO_MAPR_TIM4_REMAP          (1U << 12)
#define AFIO_MAPR_TIM3_REMAP(n)       (((n)&0x3U) << 10)
#define AFIO_MAPR_TIM2_REMAP(n)       (((n)&0x3U) << 8)
#define AFIO_MAPR_TIM1_REMAP(n)       (((n)&0x3U) << 6)
    //        Oscillator Remapping (0):
    //            PD01_REMAP (Port D0/Port D1 mapping on OSC_IN/OSC_OUT)
#define AFIO_MAPR_PD01_REMAP          (1U << 15)
    //        Communications Remapping (0):
#define AFIO_MAPR_CAN_REMAP(n)        (((n)&0x3U) << 13)
#define AFIO_MAPR_USART3_REMAP(n)     (((n)&0x3U) << 4)
#define AFIO_MAPR_USART2_REMAP        (1U << 3)
#define AFIO_MAPR_USART1_REMAP        (1U << 2)
#define AFIO_MAPR_I2C1_REMAP          (1U << 1)
#define AFIO_MAPR_SPI1_REMAP          (1U << 0)
    struct mapr MAPR;

    // ==========================================
    // External Interrupt Configuration Register 1 (AFIO_EXTICR1)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        EXTIx[3:0] (EXTI x configuration (x=0..3)) (rw) (0)
#define AFIO_EXTICR1_EXTI(x, n)   (((n)&0xFU) << ((x)&0x3)*4)
    uint32_t EXTICR1;

    // ==========================================
    // External Interrupt Configuration Register 2 (AFIO_EXTICR1):
    //    Reset value:
    //        0x0000
    //    Bits:
    //        EXTIx[3:0] (EXTI x configuration (x=4..7)) (rw) (0)
#define AFIO_EXTICR2_EXTI(x, n)   (((n)&0xFU) << ((x)&0x3)*4)
    uint32_t EXTICR2;

    // ==========================================
    // External Interrupt Configuration Register 3 (AFIO_EXTICR1):
    //    Reset value:
    //        0x0000
    //    Bits:
    //        EXTIx[3:0] (EXTI x configuration (x=8..11)) (rw) (0)
#define AFIO_EXTICR3_EXTI(x, n)   (((n)&0xFU) << ((x)&0x3)*4)
    uint32_t EXTICR3;

    // ==========================================
    // External Interrupt Configuration Register 4 (AFIO_EXTICR1):
    //    Reset value:
    //        0x0000
    //    Bits:
    //        EXTIx[3:0] (EXTI x configuration (x=12..15)) (rw) (0)
#define AFIO_EXTICR4_EXTI(x, n)   (((n)&0xFU) << ((x)&0x3)*4)
    uint32_t EXTICR4;

    // ==========================================
    // AF remap and debug I/O configuration register 2 (AFIO_MAPR2):
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        FSMC_NADV (NADV connect/disconnect) (rw) (0)
#define AFIO_MAPR2_FSMC_NADV  (1U << 10)
    //        Timer remapping (rw) (0)
#define AFIO_MAPR2_TIM14_REMAP  (1U << 9)
#define AFIO_MAPR2_TIM13_REMAP  (1U << 8)
#define AFIO_MAPR2_TIM12_REMAP  (1U << 7)
#define AFIO_MAPR2_TIM10_REMAP  (1U << 6)
#define AFIO_MAPR2_TIM9_REMAP   (1U << 5)
    struct mapr MAPR2;
};

#define AFIO ((struct afio *)0x40010000)

#endif
