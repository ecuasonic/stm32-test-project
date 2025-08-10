#include "types.h"
#include "util.h"
#include "systick.h"
#include "core_stm/gpio.h"
#include "core_stm/rcc.h"
#include "core_stm/afio.h"
#include "core_stm/exti.h"

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
#define CLEAR_CNF(pin)          (~GPIO_CRH_CNF(pin, 0x3))
#define SET_CNF(pin, cnf)       GPIO_CRH_CNF(pin, cnf)
#define SET_CNF_IN_PUPD(pin)    SET_CNF(pin, 0x2)
#define SET_CNF_OUT_PP(pin)     SET_CNF(pin, 0x0)
#define SET_CNF_OUT_AF_PP(pin)  SET_CNF(pin, 0x2)
// -- MODE
#define SET_MODE(pin, mode)     GPIO_CRH_MODE(pin, mode)
#define CLEAR_MODE(pin)         (~GPIO_CRH_MODE(pin, 0x3))
#define SET_MODE_IN(pin)        SET_MODE(pin, 0x0)
#define SET_MODE_OUT_2MHZ(pin)  SET_MODE(pin, 0x2)
#define SET_MODE_OUT_10MHZ(pin) SET_MODE(pin, 0x1)
#define SET_MODE_OUT_50MHZ(pin) SET_MODE(pin, 0x3)
// -- CNF and MODE
#define CLEAR_PIN(pin)          CLEAR_CNF(pin) & CLEAR_MODE(pin)

static void config_rcc(void) {
    // HSE Clock not connected
    // RCC->CR |= RCC_CR_HSEON;
    // while (!(RCC->CR & RCC_CR_HSERDY));

    // Only for RTC and Watchdog
    // RCC->CR |= RCC_CSR_LSION;
    // while (!(RCC->CR & RCC_CSR_LSIRDY));

    // Modify HCLK (SYSCLK/HPRE)
    RCC->CFGR &= CLEAR_HPRE; // No Prescale
    // RCC->CFGR |= SET_HPRE_2;

    // Set up PLL
    RCC->CFGR &= SET_HSI_HALF; // PLL = (HSI/2)*PLLMUL
    RCC->CFGR &= CLEAR_PLLMUL;
    RCC->CFGR |= SET_PLLMUL_5;

    // MCO
    RCC->CFGR &= CLEAR_MCO;
    RCC->CFGR |= SET_MCO_PLL;
}

static void config_gpio(void) {
    // LED Output
    GPIO('C')->CRH &= CLEAR_PIN(13);
    GPIO('C')->CRH |= SET_MODE_OUT_2MHZ(13) | SET_CNF_OUT_PP(13);

    // MCO AF Output
    GPIO('A')->CRH &= CLEAR_PIN(8);
    GPIO('A')->CRH |= SET_MODE_OUT_50MHZ(8) | SET_CNF_OUT_AF_PP(8);

    // EXTI0 Line (Input Mode Pull-Down)
    GPIO('A')->CRL &= CLEAR_PIN(0);
    GPIO('A')->CRL |= SET_MODE_IN(0) | SET_CNF_IN_PUPD(0);
    GPIO('A')->ODR = GPIO_ODR(0);
}

static void config_intr(void) {
    AFIO->EXTICR1 |= AFIO_EXTICR1_EXTI(0, 0);
    EXTI->IMR |= EXTI_MR(0);
    EXTI->RTSR |= EXTI_TR(0);
}

static void start_gpio_clocks(void) {
    // APB2
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
}

static void start_rcc_clocks(void) {
    // PLL
    RCC->CR |= RCC_CR_PLLON;
    vuint32_t *rcc_cr = &RCC->CR;
    while (!(*rcc_cr & RCC_CR_PLLRDY));
}

static void setup(void) {
    config_rcc();
    start_rcc_clocks();

    start_gpio_clocks();
    config_gpio();

    // config_intr();
}

int main(void) {
    setup();

    for (;;) {
        uint32_t timer, period = 1000;
        if (timer_expired(&timer, period, s_ticks)) {
            static uint32_t set;
            if (set) {
                gpio_set('C', 13);
            } else {
                gpio_clear('C', 13);
            }
            set = ~set;
        }
    }
}
