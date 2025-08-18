#include "types.h"
#include "defines.h"
#include "core_stm/gpio.h"
#include "core_stm/rcc.h"
#include "core_stm/afio.h"
#include "core_stm/exti.h"

static void config_rcc(void) {
}

static void start_rcc_clocks(void) {
}

// =================================================
//
static void start_gpio_clocks(void) {
    // APB2
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
}

static void config_gpio(void) {
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
    *((uint32_t *)0xE000E100) |= (1 << 6);
}

// =================================================

static void setup(void) {
    config_rcc();
    start_rcc_clocks();

    start_gpio_clocks();
    config_gpio();

    config_intr();
}

// Things to look into:
//      Backup registers
//      Sleep mode, then wake up (on event or interrupt)
//      Events
//      Power control
//      I2C
int main(void) {
    setup();

    while ((volatile uint32_t)1);
}
