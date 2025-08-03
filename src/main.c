#include "types.h"
#include "util.h"
#include "systick.h"
#include "gpio/gpio.h"
#include "rcc/rcc.h"

#define TIME 1060000

int main(void) {
    // Ensure HSI is ON is ready.
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    // Set HSI trimming if needed (optional)
    RCC->CR |= RCC_CR_HSITRIM(0x10U);

    // Configure clock: SYSCLK = HSI, AHB = no div, PB1/APB2 = no div
    RCC->CFGR &= (uint32_t)~(RCC_CFGR_SW_MASK | RCC_CFGR_HPRE_MASK |
            RCC_CFGR_PPRE1_MASK | RCC_CFGR_PPRE2_MASK | RCC_CFGR_MCO_MASK);

    RCC->CFGR |= RCC_CFGR_SW_HSI | RCC_CFGR_HPRE_NODIV |
        RCC_CFGR_PPRE1_NODIV | RCC_CFGR_PPRE2_NODIV |
        RCC_CFGR_MCO_HSICLK;

    RCC->APB2ENR |= RCC_IOPCEN;
    uint16_t led = PIN('C', 13);
    gpio_set_mode(led, GPIO_MODE_OUTPUT_2MHZ);

    for (;;) {
        uint32_t timer, period = 1000;
        if (timer_expired(&timer, period, s_ticks)) {
            static uint32_t set;
            gpio_write(led, set);
            set = ~set;
        }
    }
}
