#include "types.h"
#include "core_stm/gpio.h"

INLINE void
gpio_set(uint32_t bank, uint32_t pin) {
    struct gpio *gpio = GPIO(bank);
    gpio->BSRR = 1 << pin;
}

INLINE void
gpio_clear(uint32_t bank, uint32_t pin) {
    struct gpio *gpio = GPIO(bank);
    gpio->BSRR = 1 << (pin + 16);
}
