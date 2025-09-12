#include "core_stm/i2c.h"
#include "periph/acc.h"
#include "cortex-m3/nvic/systick.h"
#include "core_stm/gpio.h"
#include "types.h"

#define TRIES 150

uint32_t config_acc(struct acc *acc, struct i2c *i2c, uint32_t addr) {

    vint32_t tries = TRIES;
    while (tries-- && start_i2c_tx(i2c, addr));

    if (tries < 0) {
        acc->configured = 0;
        return FAILURE;
    } else {
        delay_ms(1);
        end_i2c_tx(i2c);
        acc->addr = addr;
        acc->i2c = i2c;
        acc->configured = 1;
        return SUCCESS;
    }
}

static uint32_t set_reg(struct acc *acc, uint32_t reg, uint32_t data) {
    int32_t tries = TRIES;

    uint32_t tx[2] = {reg, data};
    while (tries-- && (i2c_tx(acc->i2c, acc->addr, tx, 2) == 0));
    return (tries < 0) ? FAILURE : SUCCESS;
}

static uint32_t read_data(struct acc *acc, char port, uint32_t pin, int32_t *dest) {
    vuint32_t *gpio = &GPIO(port)->IDR;

    // Switch to data registers
    uint32_t tx = 0x3;
    CHECK_ERROR(start_i2c_tx(acc->i2c, acc->addr));
    CHECK_NULLPTR(i2c_tx(acc->i2c, NO_COND, &tx, 1));

    // Wait for DRDY
    while (!(*gpio & GPIO_ODR(pin)));

    // Get data (x,y,z)
    CHECK_NULLPTR(i2c_rx(acc->i2c, acc->addr, (uint32_t *)dest, 6));

    return SUCCESS;
}

uint32_t get_test_acc(struct acc *acc, char port, uint32_t pin, int32_t *dest) {
    // Set self-test bit
    CHECK_ERROR(set_reg(acc, 0xC, 1U << 6));

    // Request self-test
    CHECK_ERROR(set_reg(acc, 0xA,0x8));
    CHECK_ERROR(read_data(acc, port, pin, dest));

    // Clear self-test bit
    CHECK_ERROR(set_reg(acc, 0xC, 0));

    return SUCCESS;
}

uint32_t get_single_data_acc(struct acc *acc, char port, uint32_t pin, int32_t *dest) {
    // Request single measurement mode
    CHECK_ERROR(set_reg(acc, 0xA, 0x1));
    CHECK_ERROR(read_data(acc, port, pin, dest));
    return SUCCESS;
}

void format_data_acc(int32_t *raw, int32_t *offset, int32_t *formatted) {
    for (uint32_t i = 0; i < 3; i++) {
        int16_t raw_whole = (int16_t)(raw[2*i] | (raw[2*i+1] << 8));
        int16_t offset_whole = (offset) ? (int16_t)(offset[2*i] | (offset[2*i+1] << 8)) : 0;
        formatted[i] = raw_whole - offset_whole;
    }
}
