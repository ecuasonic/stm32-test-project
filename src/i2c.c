#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"
#include "types.h"
#include "periph/lcd.h"

uint32_t cursor_x;
uint32_t cursor_y;

static vuint32_t *const i2c1_dr = &I2C1->DR;
static vuint32_t *const i2c1_sr1 = &I2C1->SR1;
static vuint32_t *const i2c1_sr2 = &I2C1->SR2;
static vuint32_t *const i2c1_cr1 = &I2C1->CR1;
static vuint32_t temp = 0;

// These should return success code, timeout if failure

// (Master)
static void send_start(void) {
    *i2c1_cr1 |= I2C_CR1_START;
    while (!(*i2c1_sr1 & I2C_SR1_SB));
}

// (Master)
static void send_stop(void) {
    *i2c1_cr1 |= I2C_CR1_STOP;
}

// (Master)
static void send_address(uint32_t addr, uint32_t rd) {
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

void tx(uint32_t byte) {
    *i2c1_dr = byte;
    while (!(*i2c1_sr1 & I2C_SR1_TXE));
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
