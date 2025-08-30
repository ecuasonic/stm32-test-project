#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"
#include "types.h"
#include "periph/lcd.h"

uint32_t cursor_x;
uint32_t cursor_y;

// These should return success code, timeout if failure

static void send_start(void) {
    vuint32_t *i2c1_sr1 = &I2C1->SR1;

    // START Condition (Master)
    I2C1->CR1 |= I2C_CR1_START;
    while (!(*i2c1_sr1 & I2C_SR1_SB));
}

static void send_stop(void) {
    // STOP Condition (Master)
    I2C1->CR1 |= I2C_CR1_STOP;
}

static void send_address(uint32_t addr, uint32_t rd) {
    vuint32_t *i2c1_dr = &I2C1->DR;
    vuint32_t *i2c1_sr1 = &I2C1->SR1;
    vuint32_t *i2c1_sr2 = &I2C1->SR2;

    vuint32_t temp = 0;

    // Send PCF8574 address
    *i2c1_dr = ((addr&0x7F) << 1) | (rd&0x1);
    while (!(*i2c1_sr1 & I2C_SR1_ADDR));
    temp = *i2c1_sr2;
}

// ===================================================================

void start_comm(uint32_t addr, uint32_t rd) {
    send_start();
    send_address(addr, rd);
}

void end_comm(void) {
    send_stop();
}

// ===================================================================

void config_i2c(void) {
    // Configure timing (from trm)
    I2C1->CR2 = I2C_CR2_FREQ(0x8);
    I2C1->CCR = I2C_CCR(0x28);
    I2C1->TRISE = I2C_TRISE(0x9);

    // Enable peripheral
    I2C1->CR1 |= I2C_CR1_PE;
}
