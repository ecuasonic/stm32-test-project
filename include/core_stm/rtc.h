#ifndef RTC_H
#define RTC_H

#include "types.h"

struct rtc {
    // ==========================================
    // RTC Control Register High (RTC_CRH) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        OWIE (Overflow interrupt enable)
#define RTC_CRH_OWIE  (1U << 2)
    //        ALRIE (Alarm interrupt enable)
#define RTC_CRH_ALRIE (1U << 1)
    //        SECIE (Second interrupt enable)
#define RTC_CRH_SECIE (1U << 0)
    uint32_t CRH;

    // ==========================================
    // RTC Control Register Low (RTC_CRL)
    //    Reset value:
    //        0x0000_0020
    //    Bits:
    //        RTOFF (RTC operation OFF) (r) (1)
#define RTC_CRL_RTOFF (1U << 5)
    //        CNF (Configuration flag) (rw) (0)
#define RTC_CRL_CNF   (1U << 4)
    //        RSF (Registers synchronized flag) (rc_w0) (0)
#define RTC_CRL_RSF   (1U << 3)
    //        OWF (Overflow flag) (rc_w0) (0)
#define RTC_CRL_OWF   (1U << 2)
    //        ALRF (Alarm flag) (rc_w0) (0)
#define RTC_CRL_ALRF  (1U << 1)
    //        SECF (Second flag) (rc_w0) (0)
#define RTC_CRL_SECF  (1U << 0)
    uint32_t CRL;

    // ==========================================
    // RTC Prescalar Load Register High (RTC_PRLH) (w) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        PRL (RTC prescalar reload value high)
#define RTC_PRLH(n)   ((n)&0xFU)
    uint32_t PRLH;

    // ==========================================
    // RTC Prescalar Load Register Low (RTC_PRLL) (w)
    //    Reset value:
    //        0x8000
    //    Bits:
    //        PRL (RTC prescalar reload value low)
#define RTC_PRLL(n)   ((n)&0xFFFFU)
    uint32_t PRLL;

    // ==========================================
    // RTC Prescalar Divider Register High (RTC_DIVH) (r) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        DIV (RTC clock divider high)
#define RTC_DIVH(n)   ((n)&0xFU)
    uint32_t DIVH;

    // ==========================================
    // RTC Prescalar Divider Register Low (RTC_DIVL) (r)
    //    Reset value:
    //        0x8000
    //    Bits:
    //        DIV (RTC clock divider low)
#define RTC_DIVL(n)   ((n)&0xFFFFU)
    uint32_t DIVL;

    // ==========================================
    // RTC Counter Register High (RTC_CNTH) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        CNT (RTC counter high)
#define RTC_CNTH(n)   ((n)&0xFFFFU)
    uint32_t CNTH;

    // ==========================================
    // RTC Counter Register Low (RTC_CNTL) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        CNT (RTC counter low)
#define RTC_CNTL(n)   ((n)&0xFFFFU)
    uint32_t CNTL;

    // ==========================================
    // RTC Alarm Register High (RTC_ALRH) (w) (0)
    //    Reset value:
    //        0xFFFF
    //    Bits:
    //        ALR (RTC alarm high)
#define RTC_ALRH(n)   ((n)&0xFFFFU)
    uint32_t ALRH;

    // ==========================================
    // RTC Alarm Register Low (RTC_ALRL) (w) (0)
    //    Reset value:
    //        0xFFFF
    //    Bits:
    //        ALR (RTC alarm low)
#define RTC_ALRL(n)   ((n)&0xFFFFU)
    uint32_t ALRL;
};

#define RTC ((struct rtc *)0x40002800)

extern vuint32_t rtc_ticks;
void setup_rtc(void);
void delay_s(uint32_t s);
void sleep_s(uint32_t s);

#endif
