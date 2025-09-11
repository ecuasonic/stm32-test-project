#include "core_stm/gpio.h"
#include "defines.h"

void config_gpio(uint32_t bank, uint32_t pin, uint32_t mode, uint32_t cnf) {
    uint32_t *gpio_cr = (pin > 7) ? &GPIO(bank)->CRH : &GPIO(bank)->CRL;
    *gpio_cr &= CLEAR_PIN(pin);
    *gpio_cr |= GPIO_MODE(pin, mode) | GPIO_CNF(pin, cnf);

    if (mode == IN && (cnf&0x3)==0x2) {
        // Custom:
        //      cnf[3] = 0 (PD)
        //      cnf[3] = 1 (PU)
        uint32_t pu = cnf & (1 << 3);
        GPIO(bank)->BSRR = (pu) ? GPIO_BSRR_BS(pin) : GPIO_BSRR_BR(pin);
    }
}

void gpio_set(uint32_t bank, uint32_t pin) {
    struct gpio *gpio = GPIO(bank);
    gpio->BSRR = 1 << pin;
}

void gpio_clear(uint32_t bank, uint32_t pin) {
    struct gpio *gpio = GPIO(bank);
    gpio->BSRR = 1 << (pin + 16);
}
