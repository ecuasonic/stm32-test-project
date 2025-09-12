#include "oled_images/flower.h"
#include "oled_images/flower2.h"
#include "font.h"

#include "periph/nfc.h"
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

    // Enable I2C1 domain
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
}

static void setup_gpio(void) {
    // START pin (for Pulseview trigger)
    config_gpio('C', 13, OUT_2MHZ, OUT_PP);

    // EXTI0 Line (Input Mode Pull-Down)
    config_gpio('B', 0, IN, IN_PD);

    // EXTI1 Line (Input Mode Pull-Down)
    config_gpio('B', 1, IN, IN_PD);

    // EXTI0 LED
    config_gpio('B', 10, OUT_2MHZ, OUT_PP);

    // ============================================================
    //                          I2C1
    // ============================================================

    config_gpio('B', 6, OUT_2MHZ, OUT_AF_OD);  // SCL
    config_gpio('B', 7, OUT_2MHZ, OUT_AF_OD);  // SDA
    config_gpio('A', 5, IN, IN_FLOAT);         // Accelerator DRDY

    // ============================================================
    //                          UART
    // ============================================================

    config_gpio('A', 0, IN, IN_PU);           // CTS
    config_gpio('A', 1, OUT_2MHZ, OUT_AF_PP); // RTS
    config_gpio('A', 2, OUT_2MHZ, OUT_AF_PP); // TX
    config_gpio('A', 3, IN, IN_PU);           // RX
    config_gpio('A', 4, OUT_2MHZ, OUT_AF_PP); // CK
}

// =================================================

static void config_intr(void) {

    vuint32_t *exticr1 = &AFIO->EXTICR1;

    // ===================
    //      EXTI0
    // ===================

    *exticr1 &= CLEAR_EXTICR(0);
    *exticr1 |= AFIO_EXTICR_EXTI('B', 0); // Map EXTI0 to B0
    EXTI->PR = EXTI_MR(0);
    EXTI->IMR |= EXTI_MR(0);
    EXTI->RTSR |= EXTI_TR(0);
    NVIC->ISER[0] |= NVIC_ISER_SETENA(6); // Enable NVIC IRQ6 (EXTI0)

    // ===================
    //      EXTI1
    // ===================

    *exticr1 &= CLEAR_EXTICR(1);
    *exticr1 |= AFIO_EXTICR_EXTI('B', 1); // Map EXTI1 to B1
    EXTI->PR = EXTI_MR(1);
    EXTI->EMR |= EXTI_MR(1); // Event
    EXTI->IMR |= EXTI_MR(1); // Interrupt
    EXTI->RTSR |= EXTI_TR(1);
    NVIC->ISER[0] |= NVIC_ISER_SETENA(7); // Enable NVIC IRQ7 (EXTI1)
}


// =================================================

static struct oled oled32, oled64;
static struct lcd lcd;

static void setup(void) {
    config_rcc();
    start_rcc();
    setup_gpio();

    config_intr();

    config_i2c();   // defined in i2c.c
    config_lcd(&lcd, LCD_I2C_ADDR);   // defined in lcd.c
    config_acc();   // defined in acc.c
    config_oled(&oled32, OLED_I2C_ADDR1, OLED_ROW32);  // defined in oled.c
    config_oled(&oled64, OLED_I2C_ADDR2, OLED_ROW64);  // defined in oled.c
}

// Things to look into:
//      Backup registers
//      Power control
//      I2C
//      ADC/DAC
//          Potentiometer into ADC.
//          DAC into transistor powering LED.
//      CRC then print on LCD/USART.
//      USART to computer
int main(void) {
    setup();

    // print_acc_data_lcd(&lcd, 'A', 5, 100);
    // print_acc_test_lcd(&lcd, 'A', 5);

    delay(1000);

    print_oled(&oled32, "Printing on OLED 32");
    print_oled(&oled64, "Printing on OLED 64");
    print_lcd(&lcd, "Printing on LCD");

    delay(1000);

    clear_oled(&oled32);
    print_oled(&oled32, "Very nice\nThis finally works");
    clear_oled(&oled64);
    print_oled(&oled64, "Very nice\nThis finally works");
    set_scroll_oled(&oled64);

    delay(1000);

    unset_scroll_oled(&oled64);
    print_image_oled(&oled64, flower);

    delay(1000);

    // TODO: Use DMA to print image.
    print_image_oled(&oled64, flower2);
    config_scroll_oled(&oled64, 0, 7);
    set_scroll_oled(&oled64);

    for (;;);
}
