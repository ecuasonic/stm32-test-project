#include "types.h"
#include "defines.h"
#include "core_stm/gpio.h"
#include "core_stm/rcc.h"
#include "core_stm/afio.h"
#include "core_stm/exti.h"
#include "cortex-m3/nvic/nvic.h"
#include "cortex-m3/nvic/systick.h"

static void config_rcc(void) {
    // -- Modify HCLK (SYSCLK/HPRE)
    RCC->CFGR &= CLEAR_HPRE; // No Prescale
    // RCC->CFGR |= SET_HPRE_2;

    // -- Set up PLL
    // RCC->CFGR &= SET_HSI_HALF; // PLL = (HSI/2)*PLLMUL
    // RCC->CFGR &= CLEAR_PLLMUL;
    // RCC->CFGR |= SET_PLLMUL_5;

    // -- MCO w/ PLL
    // RCC->CFGR &= CLEAR_MCO;
    // RCC->CFGR |= SET_MCO_PLL; // MCO pulse can leak into other GPIOA traces
}

static void start_rcc_clocks(void) {
    // -- HSE Clock not connected
    // RCC->CR |= RCC_CR_HSEON;
    // while (!(RCC->CR & RCC_CR_HSERDY));

    // -- Only for RTC and Watchdog
    // RCC->CSR |= RCC_CSR_LSION;
    // vuint32_t *rcc_csr = &RCC->CSR;
    // while (!(*rcc_csr & RCC_CSR_LSIRDY));

    // -- PLLCLK
    // RCC->CR |= RCC_CR_PLLON;
    // vuint32_t *rcc_cr = &RCC->CR;
    // while (!(*rcc_cr & RCC_CR_PLLRDY));
}

// =================================================
//
static void start_periph_clocks(void) {
    // APB2
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
}

static void config_gpio(void) {
    // LED Output
    GPIO('C')->CRH &= CLEAR_PIN(13);
    GPIO('C')->CRH |= SET_MODE_OUT_2MHZ(13) | SET_CNF_OUT_PP(13);

    // MCO AF Output
    // GPIO('A')->CRH &= CLEAR_PIN(8);
    // GPIO('A')->CRH |= SET_MODE_OUT_50MHZ(8) | SET_CNF_OUT_AF_PP(8);

    // EXTI0 Line (Input Mode Pull-Down)
    GPIO('A')->CRL &= CLEAR_PIN(0);
    GPIO('A')->CRL |= SET_MODE_IN(0) | SET_CNF_IN_PUPD(0);
    GPIO('A')->BSRR = 0;

    // EXTI0 LED
    GPIO('B')->CRL &= CLEAR_PIN(0);
    GPIO('B')->CRL |= SET_MODE_OUT_2MHZ(0) | SET_CNF_OUT_PP(0);
}

// =================================================

static void config_intr(void) {
    // Map EXTI0 to A0
    AFIO->EXTICR1 &= CLEAR_EXTICR1;
    AFIO->EXTICR1 |= AFIO_EXTICR1_EXTI(0, 0);

    // Configure EXTI0
    EXTI->PR = EXTI_MR(0);
    EXTI->IMR |= EXTI_MR(0);
    EXTI->RTSR |= EXTI_TR(0);
    EXTI->FTSR |= EXTI_TR(0);

    // Enable NVIC IRQ6 (EXTI0)
    NVIC->ISER[0] = NVIC_ISER_SETENA(6);
}

// =================================================

static void setup(void) {
    config_rcc();
    start_rcc_clocks();

    start_periph_clocks();
    config_gpio();

    config_intr();
}

// Turns on on-board LED every 1000 1ms SysTick Interrupts
// EXTI0 interrupt on pin A0.
int main(void) {
    setup();

    for (;;) {
        uint32_t timer, period = 1000;
        if (timer_expired(&timer, period, s_ticks)) {
            // EXTI->SWIER = EXTI_SWIER(0); // Software interrupt from EXTI0
            static uint32_t set;
            if (set) {
                gpio_set('C', 13);
            } else {
                gpio_clear('C', 13);
            }
            set = !set;
        }
    }
}
