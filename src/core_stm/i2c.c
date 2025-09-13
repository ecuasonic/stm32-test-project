#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"
#include "core_stm/dma.h"
#include "types.h"

#include "core_stm/gpio.h"

#define TIMEOUT 100000

static vuint32_t temp = 0;

static void send_start(struct i2c *i2c) {
    vuint32_t *cr1 = &i2c->CR1;
    vuint32_t *sr1 = &i2c->SR1;
    *cr1 |= I2C_CR1_START;
    while (!(*sr1 & I2C_SR1_SB));
}

static void send_stop(struct i2c *i2c) {
    vuint32_t *cr1 = &i2c->CR1;
    *cr1 |= I2C_CR1_STOP;
}

// ===================================================================

static uint32_t send_address_rx(struct i2c *i2c, uint32_t addr, int32_t nbytes) {
    vuint32_t *dr = &i2c->DR;
    vuint32_t *cr1 = &i2c->CR1;
    vuint32_t *sr1 = &i2c->SR1;
    vuint32_t *sr2 = &i2c->SR2;

    if (nbytes < 0) {
        return FAILURE;
    }

    *dr = ((addr&0x7F) << 1) | 1;
    *cr1 |= I2C_CR1_ACK;

    if (nbytes == 2) {
        *cr1 |= I2C_CR1_POS;
    }

    vuint32_t timeout = TIMEOUT;
    while (timeout--) {

        if (*sr1 & I2C_SR1_ADDR) {
            // Clear ACK
            if (nbytes == 1) {
                *cr1 &= ~I2C_CR1_ACK;
            }

            // Clear ADDR
            temp = *sr1;
            temp = *sr2;

            // Clear ACK
            if (nbytes == 2) {
                *cr1 &= ~I2C_CR1_ACK;
            }

            return SUCCESS;
        } else if (*sr1 & I2C_SR1_AF) {
            // Clear NACK
            *sr1 &= ~I2C_SR1_AF;
            return FAILURE;
        }
    }
    return FAILURE;
}

static uint32_t *rx(struct i2c *i2c, uint32_t *dest, int32_t nbytes) {
    vuint32_t *dr = &i2c->DR;
    vuint32_t *sr1 = &i2c->SR1;

    while (nbytes--) {
        while (!(*sr1 & I2C_SR1_RXNE));
        *dest++ = *dr;
    }
    return dest;
}


static uint32_t *send_stop_rx(struct i2c *i2c, uint32_t *dest, int32_t nbytes) {
    vuint32_t *dr = &i2c->DR;
    vuint32_t *cr1 = &i2c->CR1;
    vuint32_t *sr1 = &i2c->SR1;
    vuint32_t *sr2 = &i2c->SR2;

    if (dest == 0 || nbytes < 0) {
        return 0;
    }

    if (nbytes > 2) {

        dest = rx(i2c, dest, nbytes-3);

        // Wait for data in DR and Shift Register
        while (!(*sr1 & I2C_SR1_BTF));

        // Clear ACK
        *cr1 &= ~I2C_CR1_ACK;

        // Clear ADDR
        temp = *sr1;
        temp = *sr2;

        // Read DataN-2
        *dest++ = *dr;

        send_stop(i2c);

        // Read DataN-1
        *dest++ = *dr;

        // Read DataN
        return rx(i2c, dest, 1);

    } else if (nbytes == 2) {

        // Wait for data in DR and Shift Register
        while (!(*sr1 & I2C_SR1_BTF));

        send_stop(i2c);

        // Read DR twice
        *dest++ = *dr;
        *dest = *dr;
        return dest;

    } else if (nbytes == 1){
        send_stop(i2c);
        return rx(i2c, dest, 1);
    }

    send_stop(i2c);
    return 0;
}

uint32_t *i2c_rx(struct i2c *i2c, uint32_t addr, uint32_t *dest, int32_t nbytes) {
    vuint32_t *cr2 = &i2c->CR2;
    while (*cr2 & I2C_CR2_DMAEN);

    send_start(i2c);
    if (send_address_rx(i2c, addr, nbytes)) {
        send_stop(i2c);
        return 0;
    }
    return send_stop_rx(i2c, dest, nbytes);
}

// ===================================================================

static uint32_t send_address_tx(struct i2c *i2c, uint32_t addr) {
    vuint32_t *dr = &i2c->DR;
    vuint32_t *sr1 = &i2c->SR1;
    vuint32_t *sr2 = &i2c->SR2;

    *dr = ((addr&0x7F) << 1) | 0;

    vuint32_t timeout = TIMEOUT;
    while (timeout--) {
        if (*sr1 & I2C_SR1_ADDR) {
            // ACK
            temp = *sr1;
            temp = *sr2;
            return SUCCESS;
        } else if (*sr1 & I2C_SR1_AF) {
            // NACK
            *sr1 &= ~I2C_SR1_AF;
            return FAILURE;
        }
    }
    return FAILURE;
}

static const uint32_t *tx(struct i2c *i2c, const uint32_t *src, uint32_t nbytes) {
    vuint32_t *dr = &i2c->DR;
    vuint32_t *sr1 = &i2c->SR1;

    while (nbytes--) {
        *dr = *src++;
        while (!(*sr1 & I2C_SR1_TXE));
    }
    return src;
}

const uint32_t *i2c_tx(struct i2c *i2c, uint32_t addr, const uint32_t *src, uint32_t nbytes) {
    if ((int32_t)addr != NO_COND) {
        vuint32_t *cr2 = &i2c->CR2;
        while (*cr2 & I2C_CR2_DMAEN);

        send_start(i2c);
        if (send_address_tx(i2c, addr)) {
            send_stop(i2c);
            return 0;
        }
    }

    src = tx(i2c, src, nbytes);

    if ((int32_t)addr != NO_COND) {
        send_stop(i2c);
    }
    return src;
}

uint32_t start_i2c_tx(struct i2c *i2c, uint32_t addr) {
    vuint32_t *cr2 = &i2c->CR2;
    while (*cr2 & I2C_CR2_DMAEN);

    send_start(i2c);
    CHECK_ERROR_ENDTX(send_address_tx(i2c, addr), i2c);

    return SUCCESS;
}

void end_i2c_tx(struct i2c *i2c) {
    send_stop(i2c);
}

// ===================================================================

// Make this configurable to which I2C
void setup_i2c(struct i2c *i2c) {
    // Configure timing (from trm)
    i2c->CR2 = I2C_CR2_FREQ(0x8);
    i2c->CCR = I2C_CCR(0x28);
    i2c->TRISE = I2C_TRISE(0x9);

    // Enable peripheral
    i2c->CR1 |= I2C_CR1_PE;
}

#define DMA_I2C1_TX 6
#define DMA_I2C2_TX 4
void i2c_tx_dma(struct i2c *i2c, const uint32_t *src, uint32_t n) {
    vuint32_t *cr2 = &i2c->CR2;
    while (*cr2 & I2C_CR2_DMAEN);

    uint32_t c = (i2c == I2C1) ? DMA_I2C1_TX : DMA_I2C2_TX;
    uint32_t ccr = DMA_CCR_MSIZE(2) | DMA_CCR_PSIZE(0) | DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE;
    *cr2 |= I2C_CR2_DMAEN;
    enable_dma_channel(c-1, &i2c->DR, (uint32_t *)src, n, ccr);
}

#define DMA_I2C1_RX 7
#define DMA_I2C2_RX 5
void i2c_rx_dma(struct i2c *i2c, uint32_t *dest, uint32_t n) {
    vuint32_t *cr2 = &i2c->CR2;
    while (*cr2 & I2C_CR2_DMAEN);

    uint32_t c = (i2c == I2C1) ? DMA_I2C1_RX : DMA_I2C2_RX;
    uint32_t ccr = DMA_CCR_MSIZE(2) | DMA_CCR_PSIZE(2) | DMA_CCR_MINC | DMA_CCR_TCIE;
    *cr2 |= I2C_CR2_DMAEN;
    enable_dma_channel(c-1, &i2c->DR, dest, n, ccr);
}

// ===================================================================
void i2c_scan(struct i2c *i2c) {
    vuint32_t *dr = &i2c->DR;
    vuint32_t *sr1 = &i2c->SR1;
    vuint32_t *sr2 = &i2c->SR1;

    for (uint32_t addr = 0x00; addr <= 0x7F; addr++) {
        send_start(i2c);
        *dr = (addr << 1) | 0;

        while (1) {
            if (*sr1 & I2C_SR1_ADDR) {
                // ACK
                temp = *sr1;
                temp = *sr2;
                delay_ms(1);
                break;
            } else if (*sr1 & I2C_SR1_AF) {
                // NACK
                *sr1 &= ~I2C_SR1_AF;
                break;
            }
        }
        send_stop(i2c);
    }
}
