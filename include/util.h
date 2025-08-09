#include "types.h"
#include "core_stm/gpio.h"

INLINE void delay(volatile uint32_t count) {
    while (count--) __asm__("nop");
}

INLINE void
gpio_set_mode(uint16_t pin, uint8_t mode, uint8_t cnf) {
    struct gpio *gpio = GPIO(DEC_PINBANK(pin));
    uint8_t n = DEC_PINNO(pin);
    volatile uint32_t *ctrl_reg;

    *ctrl_reg &= ~(cnf << (4*n));
    *ctrl_reg |= (mode << (4*n));
}

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
