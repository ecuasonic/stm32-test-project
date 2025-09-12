#include "types.h"
#include "defines.h"
#include "core_stm/gpio.h"
#include "core_stm/rcc.h"
#include "core_stm/afio.h"
#include "core_stm/exti.h"
#include "cortex-m3/nvic/nvic.h"
#include "cortex-m3/nvic/systick.h"
#include "cortex-m3/asm.h"

static void config_rcc(void) {
    // -- Modify HCLK (SYSCLK/HPRE)
    // RCC->CFGR &= CLEAR_HPRE; // No Prescale
    // RCC->CFGR |= SET_HPRE_2;

    // -- Set up PLL
    // RCC->CFGR &= SET_HSI_HALF; // PLL = (HSI/2)*PLLMUL
    // RCC->CFGR &= CLEAR_PLLMUL;
    // RCC->CFGR |= SET_PLLMUL_5;

    // -- MCO w/ PLL
    // RCC->CFGR &= CLEAR_MCO;
    // RCC->CFGR |= SET_MCO_PLL; // MCO pulse can leak into other GPIOA traces
}

// =================================================
//
static void start_rcc(void) {
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

    // Enable GPIO port domain
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // Enable AFIO domain
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
}

static void setup_gpio(void) {
    // MCO AF Output
    // config_gpio('A', 8, OUT_50MHZ, OUT_AF_PP);

    // SysTick LED
    config_gpio('C', 13, OUT_2MHZ, OUT_PP);

    // EXTI0 Line (Input Mode Pull-Down)
    config_gpio('B', 0, IN, IN_PD);

    // EXTI1 Line (Input Mode Pull-Down)
    config_gpio('B', 1, IN, IN_PD);

    // EXTI0 LED
    config_gpio('B', 10, OUT_2MHZ, OUT_PP);
}

// =================================================

static void config_intr(void) {

    // ===================
    //      EXTI0
    // ===================

    AFIO->EXTICR1 &= CLEAR_EXTICR(0);
    AFIO->EXTICR1 |= AFIO_EXTICR_EXTI('B', 0); // Map EXTI0 to B0
    EXTI->PR = EXTI_MR(0);
    EXTI->IMR |= EXTI_MR(0);
    EXTI->RTSR |= EXTI_TR(0);
    NVIC->ISER[0] |= NVIC_ISER_SETENA(6); // Enable NVIC IRQ6 (EXTI0)

    // ===================
    //      EXTI1
    // ===================

    AFIO->EXTICR1 &= CLEAR_EXTICR(1);
    AFIO->EXTICR1 |= AFIO_EXTICR_EXTI('B', 1); // Map EXTI1 to A1
    EXTI->PR = EXTI_MR(1);
    EXTI->EMR |= EXTI_MR(1); // Event
    EXTI->IMR |= EXTI_MR(1); // Interrupt
    EXTI->RTSR |= EXTI_TR(1);
    NVIC->ISER[0] |= NVIC_ISER_SETENA(7); // Enable NVIC IRQ7 (EXTI1)
}

// =================================================

static void setup(void) {
    config_rcc();
    start_rcc();

    setup_gpio();

    config_intr();
}

// The `main()` code stops when sleeping. Resumes when it wakes back up by event.
extern vuint32_t sleep_request;
int main(void) {
    setup();

    // 1. Put to sleep on button press
    // 2. Wakeup with another button press

    static uint32_t set;
    for (;;) {
        if (sleep_request) {
            sleep_request = 0;

            // wfe() will not go to sleep if event register is set, or pending interrupts
            cpsid();
            sev(); // set event register (no sleep)
            wfe(); // clear event register (no sleep)
            wfe(); // set event register (sleep)
            cpsie();
        }

        uint32_t timer, period = 1000;
        if (timer_expired(&timer, period, s_ticks)) {
            // EXTI->SWIER = EXTI_SWIER(0); // Software interrupt from EXTI0
            if (set) {
                gpio_set('C', 13);
            } else {
                gpio_clear('C', 13);
            }
            set = !set;
        }
    }
}
