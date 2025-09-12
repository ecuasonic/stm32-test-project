#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"
#include "types.h"
#include "periph/lcd-hd44780u.h"

#define TIMEOUT 100000

static vuint32_t *const i2c1_dr = &I2C1->DR;
static vuint32_t *const i2c1_sr1 = &I2C1->SR1;
static vuint32_t *const i2c1_sr2 = &I2C1->SR2;
static vuint32_t *const i2c1_cr1 = &I2C1->CR1;
static vuint32_t temp = 0;

static void send_start(void) {
    *i2c1_cr1 |= I2C_CR1_START;
    while (!(*i2c1_sr1 & I2C_SR1_SB));
}

static void send_stop(void) {
    *i2c1_cr1 |= I2C_CR1_STOP;
}

// ===================================================================

static uint32_t send_address_rx(uint32_t addr, int32_t nbytes) {
    if (nbytes < 0) {
        return FAILURE;
    }

    *i2c1_dr = ((addr&0x7F) << 1) | 1;
    *i2c1_cr1 |= I2C_CR1_ACK;

    if (nbytes == 2) {
        *i2c1_cr1 |= I2C_CR1_POS;
    }

    vuint32_t timeout = TIMEOUT;
    while (timeout--) {

        if (*i2c1_sr1 & I2C_SR1_ADDR) {
            // Clear ACK
            if (nbytes == 1) {
                *i2c1_cr1 &= ~I2C_CR1_ACK;
            }

            // Clear ADDR
            temp = *i2c1_sr1;
            temp = *i2c1_sr2;

            // Clear ACK
            if (nbytes == 2) {
                *i2c1_cr1 &= ~I2C_CR1_ACK;
            }

            return SUCCESS;
        } else if (*i2c1_sr1 & I2C_SR1_AF) {
            // Clear NACK
            *i2c1_sr1 &= ~I2C_SR1_AF;
            return FAILURE;
        }
    }
    return FAILURE;
}

static uint32_t *rx(uint32_t *dest, int32_t nbytes) {
    while (nbytes--) {
        while (!(*i2c1_sr1 & I2C_SR1_RXNE));
        *dest++ = *i2c1_dr;
    }
    return dest;
}


static uint32_t *send_stop_rx(uint32_t *dest, int32_t nbytes) {
    if (dest == 0 || nbytes < 0) {
        return 0;
    }

    if (nbytes > 2) {

        dest = rx(dest, nbytes-3);

        // Wait for data in DR and Shift Register
        while (!(*i2c1_sr1 & I2C_SR1_BTF));

        // Clear ACK
        *i2c1_cr1 &= ~I2C_CR1_ACK;

        // Clear ADDR
        temp = *i2c1_sr1;
        temp = *i2c1_sr2;

        // Read DataN-2
        *dest++ = *i2c1_dr;

        send_stop();

        // Read DataN-1
        *dest++ = *i2c1_dr;

        // Read DataN
        return rx(dest, 1);

    } else if (nbytes == 2) {

        // Wait for data in DR and Shift Register
        while (!(*i2c1_sr1 & I2C_SR1_BTF));

        send_stop();

        // Read DR twice
        *dest++ = *i2c1_dr;
        *dest = *i2c1_dr;
        return dest;

    } else if (nbytes == 1){
        send_stop();
        return rx(dest, 1);
    }

    send_stop();
    return 0;
}

uint32_t *i2c_rx(uint32_t addr, uint32_t *dest, int32_t nbytes) {
    send_start();
    if (send_address_rx(addr, nbytes)) {
        send_stop();
        return 0;
    }
    return send_stop_rx(dest, nbytes);
}

// ===================================================================

static uint32_t send_address_tx(uint32_t addr) {
    *i2c1_dr = ((addr&0x7F) << 1) | 0;

    vuint32_t timeout = TIMEOUT;
    while (timeout--) {
        if (*i2c1_sr1 & I2C_SR1_ADDR) {
            // ACK
            temp = *i2c1_sr1;
            temp = *i2c1_sr2;
            return SUCCESS;
        } else if (*i2c1_sr1 & I2C_SR1_AF) {
            // NACK
            *i2c1_sr1 &= ~I2C_SR1_AF;
            return FAILURE;
        }
    }
    return FAILURE;
}

static const uint32_t *tx(const uint32_t *src, uint32_t nbytes) {
    while (nbytes--) {
        *i2c1_dr = *src++;
        while (!(*i2c1_sr1 & I2C_SR1_TXE));
    }
    return src;
}

const uint32_t *i2c_tx(uint32_t addr, const uint32_t *src, uint32_t nbytes) {
    if ((int32_t)addr != NO_COND) {
        send_start();
        if (send_address_tx(addr)) {
            send_stop();
            return 0;
        }
    }

    src = tx(src, nbytes);

    if ((int32_t)addr != NO_COND) {
        send_stop();
    }
    return src;
}

uint32_t start_i2c_tx(uint32_t addr) {
    send_start();
    CHECK_ERROR_ENDTX(send_address_tx(addr));

    return SUCCESS;
}

void end_i2c_tx(void) {
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

// ===================================================================
void i2c_scan(void) {
    for (uint32_t addr = 0x00; addr <= 0x7F; addr++) {
        send_start();
        *i2c1_dr = (addr << 1) | 0;

        while (1) {
            if (*i2c1_sr1 & I2C_SR1_ADDR) {
                // ACK
                temp = *i2c1_sr1;
                temp = *i2c1_sr2;
                delay(1);
                break;
            } else if (*i2c1_sr1 & I2C_SR1_AF) {
                // NACK
                *i2c1_sr1 &= ~I2C_SR1_AF;
                break;
            }
        }
        send_stop();
    }
}
