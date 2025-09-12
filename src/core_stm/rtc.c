#include "core_stm/rtc.h"
#include "core_stm/pwr.h"
#include "core_stm/rcc.h"
#include "cortex-m3/nvic/nvic.h"
#include "cortex-m3/asm.h"
#include "types.h"

// Dont forget to set 1 second
void config_rtc(void) {
    vuint32_t *rtc_crh = &RTC->CRH;
    vuint32_t *rtc_crl = &RTC->CRL;
    vuint32_t *rtc_prlh = &RTC->PRLH;
    vuint32_t *rtc_prll = &RTC->PRLL;
    vuint32_t *rcc_bdcr = &RCC->BDCR;

    // Disable BKP protection
    PWR->CR |= PWR_CR_DBP;

    *rcc_bdcr |= RCC_BDCR_LSEON;
    while (!(*rcc_bdcr & RCC_BDCR_LSERDY));

    *rcc_bdcr |= RCC_BDCR_RTCSEL(1);    // Set RTC source as LSE (36.768 kHz)
    *rcc_bdcr |= RCC_BDCR_RTCEN;        // Enable RTC

    // ===========================================

    // Wait for registers to be ready
    while (!(*rtc_crl & RTC_CRL_RTOFF));
    *rtc_crl |= RTC_CRL_CNF;

    // 1 second period
    *rtc_prlh = RTC_PRLH(0x00);
    *rtc_prll = RTC_PRLL(0x7FFF);

    // Enable second interrupt
    *rtc_crh |= RTC_CRH_SECIE;

    // Wait for registers to be written to
    *rtc_crl &= ~RTC_CRL_CNF;
    while (!(*rtc_crl & RTC_CRL_RTOFF));

    // ===========================================

    // Enable NVIC IRQ5 (RCC global interrupt)
    NVIC->ISER[0] |= NVIC_ISER_SETENA(3);

    // Enable BKP write protection.
    // PWR->CR &= ~PWR_CR_DBP;
}

// Use if minutes/hours
void delay_s(uint32_t s) {
    uint32_t now = rtc_ticks;
    while (rtc_ticks - now < s);
}

void sleep_s(uint32_t s) {
    uint32_t now = rtc_ticks;
    while (rtc_ticks - now < s) {
        sev(); // set event register (no sleep)
        wfe(); // clear event register (no sleep)
        wfe(); // set event register (sleep)
    }
}

// TODO: Make function to give seconds, minutes, hours
