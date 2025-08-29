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
}

static void start_rcc_clocks(void) {
    // RCC->CSR |= RCC_CSR_LSION;
    // vuint32_t *rcc_csr = &RCC->CSR;
    // while (!(*rcc_csr & RCC_CSR_LSIRDY));
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
    // SysTick LED
    GPIO('C')->CRH &= CLEAR_PIN(13);
    GPIO('C')->CRH |= SET_MODE_OUT_2MHZ(13) | SET_CNF_OUT_PP(13);

    // EXTI0 Line (Input Mode Pull-Down)
    GPIO('A')->CRL &= CLEAR_PIN(0);
    GPIO('A')->CRL |= SET_MODE_IN(0) | SET_CNF_IN_PUPD(0);
    GPIO('A')->BSRR = GPIO_BSRR_BR(0);

    // EXTI1 Line (Input Mode Pull-Down)
    GPIO('A')->CRL &= CLEAR_PIN(1);
    GPIO('A')->CRL |= SET_MODE_IN(1) | SET_CNF_IN_PUPD(1);
    GPIO('A')->BSRR = GPIO_BSRR_BR(1);

    // EXTI0 LED
    GPIO('B')->CRL &= CLEAR_PIN(0);
    GPIO('B')->CRL |= SET_MODE_OUT_2MHZ(0) | SET_CNF_OUT_PP(0);
}

// =================================================

static void config_intr(void) {
    AFIO->EXTICR1 &= CLEAR_EXTICR1;

    // ===================
    //      EXTI0
    // ===================

    AFIO->EXTICR1 |= AFIO_EXTICR1_EXTI(0, 0); // Map EXTI0 to A0
    // Configure EXTI0
    EXTI->PR = EXTI_MR(0);
    EXTI->IMR |= EXTI_MR(0);
    EXTI->RTSR |= EXTI_TR(0);
    NVIC->ISER[0] |= NVIC_ISER_SETENA(6); // Enable NVIC IRQ6 (EXTI0)

    // ===================
    //      EXTI1
    // ===================

    AFIO->EXTICR1 |= AFIO_EXTICR1_EXTI(1, 0); // Map EXTI1 to A1
    // Configure EXTI1
    EXTI->PR = EXTI_MR(1);
    EXTI->EMR |= EXTI_MR(1); // Event
    EXTI->IMR |= EXTI_MR(1); // Interrupt
    EXTI->RTSR |= EXTI_TR(1);
    NVIC->ISER[0] |= NVIC_ISER_SETENA(7); // Enable NVIC IRQ7 (EXTI1)
}

// =================================================

static void setup(void) {
    config_rcc();
    start_rcc_clocks();

    start_periph_clocks();
    config_gpio();

    config_intr();

    // ==============
    gpio_set('B', 0);
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
