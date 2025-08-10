#ifndef BKP_H
#define BKP_H

#include "types.h"

struct bkp {
    // ==========================================
    // Backup Data Register x (BKP_DRx) (x = 1..42) (rw) (0)
    //    Reset value:
    //        0x0000_0000
#define BKP_DR(n)   ((n)&0xFFFFU)
    uint32_t DR_A[10];

    // ==========================================
    // RTC Clock Calibration Register (BKP_RTCCR) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Description:
    //        ASOS (Alarm or Second Output Selection)
#define BKP_RTCCR_ASOS (1U << 9)
    //        ASOE (Alarm or Second Output Enable)
#define BKP_RTCCR_ASOE (1U << 8)
    //        COO (Calibration Clock Output)
#define BKP_RTCCR_CCO  (1U << 7)
    //        CAL (Calibration value)
#define BKP_RTCCR_CAL(n) ((uint32_t)n << 9)
    uint32_t BKP_RTCCR;

    // ==========================================
    // Backup Control Register (BKP_CR) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Description:
    //        TPAL (TAMPER pin active level)
#define BKP_CR_TPAL (1U << 1)
    //        TPE (TAMPER pin enable)
#define BKP_CR_TPE (1U << 0)
    uint32_t BKP_CR;

    // ==========================================
    // Backup control/status register (BKP_CSR)
    //    Reset value:
    //        0x0000_0000
    //    Description:
    //        TIF (Tamper interrupt flag) (r) (0)
#define BKP_CSR_TIF (1U << 9)
    //        TEF (Tamper event flag) (r) (0)
#define BKP_CSR_TEF (1U << 8)
    //        TPIE (TAMPER pin interrupt enable) (rw) (0)
#define BKP_CSR_TPIE (1U << 2)
    //        CTI (Clear tamper interrupt) (w) (0)
#define BKP_CSR_CTI (1U << 1)
    //        CTE (Clear tamper event) (w) (0)
#define BKP_CSR_CTE (1U << 0)
    uint32_t BKP_CSR;

    uint32_t RES0;
    uint32_t RES1;

    // ==========================================
    // Backup Data Register x (BKP_DRx) (x = 1..42) (rw) (0)
    //    Reset value:
    //        0x0000_0000
#define BKP_DR(n)   ((n)&0xFFFFU)
    uint32_t DR_B[32];
};

#define BKP     ((struct bkp *)0x40006C00)

#endif
