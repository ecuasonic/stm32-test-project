#include "types.h"
#include "util.h"
#include "systick.h"
#include "gpio/gpio.h"
#include "rcc.h"

#define TIME 1060000

#define RCC_CFGR_MCO_HSI    RCC_CFGR_MCO(0b101)
#define RCC_CFGR_SW_HSE     RCC_CFGR_SW(0b01)

void config_rcc(void) {
    // Clock Control Register
    //      HSI is on by default.
    RCC->CR |= RCC_CR_HSEON;
    RCC->CR |= RCC_CR_HSITRIM(0x10U);

    // Clock Configuration Register
    RCC->CFGR |= RCC_CFGR_MCO_HSI;
    RCC->CFGR |= RCC_CFGR_SW_HSE;

    // APB2 Peripheral Clock Enable Register
    RCC->APB2ENR |= RCC_APB2ENR_IOCEN;
}

void setup(void) {
    config_rcc();
}

int main(void) {
    setup();

    uint16_t led = PIN('C', 13);
    gpio_set_mode(led, GPIO_MODE_OUTPUT_2MHZ);

    for (;;) {
        uint32_t timer, period = 1000;
        if (timer_expired(&timer, period, s_ticks)) {
            static uint32_t set = 0;
            gpio_write(led, set);
            set = ~set;
        }
    }
}
