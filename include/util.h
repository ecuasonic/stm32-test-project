#include "types.h"
#include "gpio/gpio.h"

INLINE void delay(volatile uint32_t count) {
    while (count--) __asm__("nop");
}

INLINE void
gpio_set_mode(uint16_t pin, uint8_t mode ) {
    // NOTE: You can also just directly index into CRL (since CRH follows after)
    // But I will have this here, since I'm am just starting out.
    struct gpio *gpio = GPIO(PINBANK(pin));
    uint8_t n = PINNO(pin);
    volatile uint32_t *ctrl_reg;
    if (n >= 8) {
        ctrl_reg = &gpio->CRH;
        n -= 8;
    } else {
        ctrl_reg = &gpio->CRL;
    }

    *ctrl_reg &= ~(0xF << (4*n));
    *ctrl_reg |= (mode << (4*n));
}

INLINE void
gpio_write(uint16_t pin, uint32_t set) {
    struct gpio *gpio = GPIO(PINBANK(pin));
    if (set) {
        gpio->BSRR = 1 << PINNO(pin);
    } else {
        gpio->BSRR = 1 << (PINNO(pin) + 16);
    }
}

