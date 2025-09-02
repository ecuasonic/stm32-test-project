#include "types.h"
#include "defines.h"
#include "core_stm/gpio.h"
#include "core_stm/rcc.h"
#include "core_stm/afio.h"
#include "core_stm/exti.h"
#include "core_stm/i2c.h"
#include "cortex-m3/nvic/nvic.h"
#include "cortex-m3/nvic/systick.h"
#include "strings.h"

#include "periph/lcd-hd44780u.h"
#include "periph/oled-ssd1306.h"
#include "periph/acc.h"

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
    // APB2 for GPIOs
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // APB1 for I2C
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
}

static void config_gpio(void) {
    // START pin (for Pulseview trigger)
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

    // I2C Acceleartor DRDY
    GPIO('A')->CRL &= CLEAR_PIN(5);
    GPIO('A')->CRL |= SET_MODE_IN(5) | SET_CNF_IN_FLOAT(5);

    // EXTI0 LED
    GPIO('B')->CRL &= CLEAR_PIN(0);
    GPIO('B')->CRL |= SET_MODE_OUT_2MHZ(0) | SET_CNF_OUT_PP(0);

    // ============================================================

    // I2C1 SCL
    GPIO('B')->CRL &= CLEAR_PIN(6);
    GPIO('B')->CRL |= SET_MODE_OUT_2MHZ(6) | SET_CNF_OUT_AF_OD(6);

    // I2C1 SDA
    GPIO('B')->CRL &= CLEAR_PIN(7);
    GPIO('B')->CRL |= SET_MODE_OUT_2MHZ(7) | SET_CNF_OUT_AF_OD(7);
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

    config_i2c();   // defined in i2c.c
    config_lcd();   // defined in lcd.c
    config_oled();  // defined in oled.c
    config_acc();   // defined in acc.c
}

// Things to look into:
//      Backup registers
//      Power control
//      I2C
//      ADC/DAC
//          Potentiometer into ADC.
//          DAC into transistor powering LED.
int main(void) {
    setup();

    print_acc_data_lcd('A', 5, 100);
    print_acc_test_lcd('A', 5);


    for (;;) {
    }
}
