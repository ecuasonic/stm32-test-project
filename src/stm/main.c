#include "oled_images/flower.h"
#include "oled_images/flower2.h"

#include "types.h"
#include "core_stm/gpio.h"
#include "core_stm/rcc.h"
#include "core_stm/exti.h"
#include "core_stm/i2c.h"
#include "cortex-m3/nvic/nvic.h"

#include "cortex-m3/nvic/systick.h"
#include "core_stm/rtc.h"

#include "periph/lcd-hd44780u.h"
#include "periph/oled-ssd1306.h"
#include "periph/acc.h"
#include "periph/nfc.h"

static void setup_rcc(void) {
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
    // -- HSI Clock (8 MHz) (Default)

    // -- HSE Clock (4-16 MHz)
    // vuint32_t *rcc_cr = &RCC->CR;
    // *rcc_cr |= RCC_CR_HSEON;
    // while (!(*rcc_cr & RCC_CR_HSERDY));

    // -- LSI Clock (40 kHz)
    // vuint32_t *rcc_csr = &RCC->CSR;
    // *rcc_csr |= RCC_CSR_LSION;
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

    // Enable PWR/BKP domain
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;

    // Enable DMA domain
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    RCC->AHBENR |= RCC_AHBENR_DMA2EN;
}

static void setup_gpio(void) {
    // ============================================================
    //                           LED
    // ============================================================

    config_gpio('C', 13, OUT_2MHZ, OUT_PP); // RTC Clock LED
    config_gpio('B', 10, OUT_2MHZ, OUT_PP); // EXTI0 LED

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

static void setup_intr(void) {
    // EXTI0 to B0
    config_gpio('B', 0, IN, IN_PD);
    config_exti(0, 'B', IMR_bit | RTSR_bit);
    NVIC->ISER[0] |= NVIC_ISER_SETENA(6); // Enable NVIC IRQ6 (EXTI0)

    // EXTI1 to B1
    config_gpio('B', 1, IN, IN_PD);
    config_exti(1, 'B', EMR_bit | IMR_bit | RTSR_bit);
    NVIC->ISER[0] |= NVIC_ISER_SETENA(7); // Enable NVIC IRQ7 (EXTI1)

    // DMA intr
    NVIC->ISER[0] |= NVIC_ISER_SETENA(14); // Enable DMA1_Channel4
    NVIC->ISER[0] |= NVIC_ISER_SETENA(16); // Enable DMA1_Channel6
}

// =================================================

static struct oled oled32, oled64;
static struct lcd lcd;
static struct acc acc;

static void setup(void) {
    setup_rcc();
    start_rcc();
    setup_gpio();

    setup_intr();

    gpio_set('B', 10);
    setup_i2c(I2C1);                                            // defined in i2c.c
    delay_ms(4);
    config_lcd(&lcd, I2C1, LCD_I2C_ADDR);                       // defined in lcd.c
    delay_ms(4);
    config_acc(&acc, I2C1, ACC_I2C_ADDR);                       // defined in acc.c
    delay_ms(1);
    config_oled(&oled32, I2C1, OLED_I2C_ADDR1, OLED_ROW32);     // defined in oled.c
    delay_ms(1);
    config_oled(&oled64, I2C1, OLED_I2C_ADDR2, OLED_ROW64);     // defined in oled.c
    delay_ms(1);

    gpio_clear('B', 10);
    setup_rtc();   // defined in rtc.c
    gpio_set('B', 10);
}

// Things to look into:
//      Scheduler
//      Backup registers
//      Power control
//      ADC/DAC
//          Potentiometer into ADC.
//          DAC into transistor powering LED.
//      CRC then print on LCD/USART.
//      USART to computer through Uart-USB
int main(void) {
    setup();

    // print_acc_data_lcd(&lcd, &acc, 'A', 5, 100);
    // print_acc_test_lcd(&lcd, &acc, 'A', 5);
    //
    // sleep_ms(1000);

    ASSERT(print_oled(&oled32, "Printing on OLED 32"));
    ASSERT(print_oled(&oled64, "Printing on OLED 64"));
    ASSERT(print_lcd(&lcd, "Printing on LCD"));

    sleep_ms(1000);

    ASSERT(clear_oled(&oled32));
    ASSERT(print_oled(&oled32, "DMA works\nOLED is done"));
    ASSERT(clear_oled(&oled64));
    ASSERT(print_oled(&oled64, "Very nice\nThis finally works\nOLED is done"));

    sleep_ms(1000);

    ASSERT(print_image_oled_dma(&oled64, flower));

    sleep_ms(1000);

    // FIX: After a long time, a second image looks unrecognizable.
    ASSERT(print_image_oled_dma(&oled64, flower2));

    ASSERT(oled_cmd_setup_hort_scroll(&oled64));
    ASSERT(oled_cmd_toggle_scroll(&oled64));

    for (;;) {
        delay_ms(1000);
        ASSERT(oled_cmd_toggle_scroll(&oled64));
        delay_ms(1000);
        ASSERT(oled_cmd_setup_hort_scroll(&oled64));
        ASSERT(oled_cmd_toggle_scroll(&oled64));
    }
}
