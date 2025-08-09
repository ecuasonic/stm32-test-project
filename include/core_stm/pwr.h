#ifndef PWR_H
#define PWR_H

#include "types.h"

struct pwr {
    // PWR_CR (Power Control Register):
    //    Reset:
    //        0x0000_0000
    //        Reset by wakeup from Standby mode.
    //    Bits:
    //        DBP (Disable backup domain write protection)
#define PWR_CR_DBP (1 << 8)
    //        PLS (PVD (Programmable Voltage Detector) Level Selection)
#define PWR_CR_PLS(n) ((n & 0b11) << 5)
    //        PVDE (Programmable Voltage Detector Enable)
#define PWR_CR_PVDE (1 << 4)
    //        CSBF (Clear Standby Flag)
#define PWR_CR_CSBF (1 << 3)
    //        CWUF (Clear Wakeup Flag)
#define PWR_CR_CWUF (1 << 2)
    //        PDDS (Power Down Deepsleep)
#define PWR_CR_PDDS (1 << 1)
    //        LPDS (Low-Power Deepsleep)
#define PWR_CR_LPDS (1 << 0)
    uint32_t CR;

    // PWR_CSR (Power Control/Status Register):
    //    Reset:
    //        0x0000_0000
    //        Not reset by wakeup from Standby mode.
    //    Description:
    //        EWUP (Enable WKUP Pin)
#define PWR_CSR_EWUP (1 << 8)
    //        PVDO (PVD Output)
#define PWR_CSR_PVDO (1 << 2)
    //        SBF (Standby Flag)
#define PWR_CSR_SBF  (1 << 1)
    //        WUF (Wakeup Flag)
#define PWR_CSR_WUF  (1 << 0)
    uint32_t CSR;
};

#define PWR ((struct pwr *)0x40007000)

#endif
