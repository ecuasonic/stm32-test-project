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
    set_gpio('C', 13, OUT_2MHZ, OUT_PP);

    // EXTI0 Line (Input Mode Pull-Down)
    set_gpio('A', 0, IN, IN_PD);

    // EXTI1 Line (Input Mode Pull-Down)
    set_gpio('A', 1, IN, IN_PD);

    // EXTI0 LED
    set_gpio('B', 0, OUT_2MHZ, OUT_PP);

    // ============================================================
    //                          I2C1
    // ============================================================

    set_gpio('B', 6, OUT_2MHZ, OUT_AF_OD);  // SCL
    set_gpio('B', 7, OUT_2MHZ, OUT_AF_OD);  // SDA
    set_gpio('A', 5, IN, IN_FLOAT);         // Accelerator DRDY

    // ============================================================
    //                          UART
    // ============================================================

    set_gpio('B', 10, OUT_2MHZ, OUT_AF_PP); // TX
    set_gpio('B', 11, IN, IN_PU);           // RX
    set_gpio('B', 12, OUT_2MHZ, OUT_AF_PP); // CK
    set_gpio('B', 13, IN, IN_PU);           // CTS
    set_gpio('B', 14, OUT_2MHZ, OUT_AF_PP); // RTS
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

static struct oled oled32, oled64;
static struct lcd lcd;

static void setup(void) {
    config_rcc();
    start_rcc_clocks();

    start_periph_clocks();
    config_gpio();

    config_intr();

    // Add structs here to assign to use struct instead of addr
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
