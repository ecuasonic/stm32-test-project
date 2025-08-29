#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"
#include "types.h"
#include "periph/lcd.h"

#define LCD_I2C_ADDR 0x27
#define RX 1
#define TX 0

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

static void start_comm(uint32_t addr, uint32_t rd) {
    send_start();
    send_address(addr, rd);
}

static void end_comm(void) {
    send_stop();
}

// ===================================================================

static void tx(uint32_t byte) {
    vuint32_t *i2c1_dr = &I2C1->DR;
    vuint32_t *i2c1_sr1 = &I2C1->SR1;

    *i2c1_dr = byte;

    *i2c1_dr |= LCD_E;
    while (!(*i2c1_sr1 & I2C_SR1_TXE));

    *i2c1_dr &= ~LCD_E;
    while (!(*i2c1_sr1 & I2C_SR1_TXE));
}

static void tx_data(uint32_t data) {
    tx(LCD_BL | LCD_RS | (data & 0xF0));
    tx(LCD_BL | LCD_RS | ((data & 0x0F) << 4));
}

static void tx_inst(uint32_t inst) {
    tx(LCD_BL | (inst & 0xF0));
    tx(LCD_BL | ((inst & 0x0F) << 4));
}

static void tx_nstr(char *str, uint32_t n) {
    char *c = str;
    while (n-- && *c) {
        tx_data(*c++);
    }
}

// ===================================================================

void config_i2c(void) {
    // Configure timing (from trm)
    I2C1->CR2 = I2C_CR2_FREQ(0x8);
    I2C1->CCR = I2C_CCR(0x28);
    I2C1->TRISE = I2C_TRISE(0x9);

    // Enable peripheral
    I2C1->CR1 |= I2C_CR1_PE;

    // Set up LCD settings
    delay(5);
    start_comm(LCD_I2C_ADDR, TX);
    tx(LCD_DATA(0x2));      // Set to 4-bit operation
    tx_inst(0x2C);          // 2 lines, 5x10 dots
    tx_inst(0x0E);          // Display on, cursor on, blinking
    tx_inst(0x01);          // Clear screen
    end_comm();
    delay(5);
}

void print(char *str) {
    start_comm(LCD_I2C_ADDR, TX);
    tx_nstr(str, LCD_MAX_CHARS);
    end_comm();
    delay(5);
}

void clear(void) {
    start_comm(LCD_I2C_ADDR, TX);
    tx_inst(0x01);
    end_comm();
    delay(5);
}
