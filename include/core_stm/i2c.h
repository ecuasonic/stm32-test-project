#ifndef I2C
#define I2C

#include "types.h"

struct i2c {
    // ==========================================
    // I2C Control Register 1 (I2C_CR1) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        SWRST (Software reset)
#define I2C_CR1_SWRST     (1U << 14)
    //        ALERT (SMBus alert)
#define I2C_CR1_ALERT     (1U << 13)
    //        PEC (Packet error checking)
#define I2C_CR1_PEC       (1U << 12)
    //        POS (ACK/PEC Position for data reception)
#define I2C_CR1_POS       (1U << 11)
    //        ACK (ACK enable)
#define I2C_CR1_ACK       (1U << 10)
    //        STOP (Stop generation)
#define I2C_CR1_STOP      (1U << 9)
    //        START (Start generation)
#define I2C_CR1_START     (1U << 8)
    //        NOSTRETCH (Clock stretching disable [Slave mode])
#define I2C_CR1_NOSTRETCH (1U << 7)
    //        ENGC (General call enable)
#define I2C_CR1_ENGC      (1U << 6)
    //        ENPEC (PEC enable)
#define I2C_CR1_ENPEC     (1U << 5)
    //        ENARP (ARP enable)
#define I2C_CR1_ENARP     (1U << 4)
    //        SMBTYPE (SMBus type)
#define I2C_CR1_SMBTYPE   (1U << 3)
    //        SMBUS (SMBus mode)
#define I2C_CR1_SMBUS     (1U << 1)
    //        PE (Peripheral mode)
#define I2C_CR1_PE        (1U << 0)
    uint32_t CR1;

    // ==========================================
    // I2C Control Register 2 (I2C_CR2) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        LAST (DMA last transfer)
#define I2C_CR2_LAST      (1U << 12)
    //        DMAEN (DMA requests enable)
#define I2C_CR2_DMAEN     (1U << 11)
    //        ITBUFEN (Buffer interrupt enable)
#define I2C_CR2_ITBUFEN   (1U << 10)
    //        ITEVTEN (Event interupt enable)
#define I2C_CR2_ITEVTEN   (1U << 9)
    //        ITERREN (Error interrupt enable)
#define I2C_CR2_ITERREN   (1U << 8)
    //        FREQ (Peripheral clock frequency)
#define I2C_CR2_FREQ(n)   (((n)&0x3FU) << 0)
    uint32_t CR2;

    // ==========================================
    // I2C Own Address Register 1 (I2C_OAR1) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        ADDMODE (Addressing mode [slave mode])
#define I2C_OAR1_ADDMODE      (1U << 15)
    //        ADD (Interface address)
#define I2C_OAR1_ADD10    (0x3FFU << 0)
#define I2C_OAR1_ADD7     (0x7FU << 1)
    uint32_t OAR1;

    // ==========================================
    // I2C Own Address Register 2 (I2C_OAR2) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        ADD (Interface address)
#define I2C_OAR2_ADD7     (0x7FU << 1)
    //        ENDUAL (Dual addressing mode enable)
#define I2C_OAR2_ENDUAL   (1U << 0)
    uint32_t OAR2;

    // ==========================================
    // I2C Data Register (I2C_DR) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        DR (8-bit data register)
#define I2C_DR(n)     ((n)&0xFF)
    uint32_t DR;

    // ==========================================
    // I2C Status Register 1 (I2C_SR1) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        -------------- (rc_w0) ----------------
    //        SMBALERT (SMBus Alert)
#define I2C_SR1_SMBALERT  (1U << 15)
    //        TIMEOUT (Timeout or Tlow error)
#define I2C_SR1_TIMEOUT   (1U << 14)
    //        PECERR (PEC Error in reception)
#define I2C_SR1_PECERR    (1U << 12)
    //        OVR (Overrun/Underrun)
#define I2C_SR1_OVR       (1U << 11)
    //        AF (Acknowledgement failure)
#define I2C_SR1_AF        (1U << 10)
    //        ARLO (Arbitration failure [master mode])
#define I2C_SR1_ARLO      (1U << 9)
    //        BERR (Bus error)
#define I2C_SR1_BERR      (1U << 8)
    //        ---------------- (r) ---------------
    //        TxE (Data register empty [transmitters])
#define I2C_SR1_TXE       (1U << 7)
    //        RxNE (Data register not empty [recievers])
#define I2C_SR1_RXNE      (1U << 6)
    //        STOPF (Stop detection [slave mode])
#define I2C_SR1_STOPF     (1U << 4)
    //        ADD10 (10-bit header sent [Master mode])
#define I2C_SR1_ADD10     (1U << 3)
    //        BTF (Byte transfer finished)
#define I2C_SR1_BTF       (1U << 2)
    //        ADDR (Address sent [master mode] / Address matched [slave mode])
#define I2C_SR1_ADDR      (1U << 1)
    //        SB (Start bit [master mode])
#define I2C_SR1_SB        (1U << 0)
    uint32_t SR1;

    // ==========================================
    // I2C Status Register 2 (I2C_SR2) (r) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        PEC (Packet error checking register)
#define I2C_SR1_PEC(n)        (((n)&0xFF) << 8)
    //        DUALF (Dual flag [slave mode])
#define I2C_SR1_DUALF         (1U << 7)
    //        SMBHOST (SMBus host header [slave mode])
#define I2C_SR1_SMBHOST       (1U << 6)
    //        SMBDEFAULT (SMBus device default address [slave mode])
#define I2C_SR1_SMBDEFAULT    (1U << 5)
    //        GENCALL (General call address [slave mode])
#define I2C_SR1_GENCALL       (1U << 4)
    //        TRA (Transmitter/reciever)
#define I2C_SR1_TRA           (1U << 2)
    //        BUSY (Bus busy)
#define I2C_SR1_BUSY          (1U << 1)
    //        MSL (Master/slave)
#define I2C_SR1_MSL           (1U << 0)
    uint32_t SR2;

    // ==========================================
    // I2C Clock Control Register (I2C_CCR) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        F/S (Master mode selection)
#define I2C_SR1_FS        (1U << 15)
    //        DUTY (Fm Master mode duty cycle)
#define I2C_SR1_DUTY      (1U << 14)
    //        CCR (Clock control register in Fm/Sm Master mode)
#define I2C_CCR(n)        (((n)&0xFFFU) << 0)
    uint32_t CCR;

    // ==========================================
    // I2C TRISE Register (I2C_TRISE) (rw) (0)
    //    Reset value:
    //        0x0002
    //    Bits:
    //        TRISE (Maximum rise time in Fm/Sm Master mode)
#define I2C_TRISE(n)  (((n)&0x3FU) << 0)
    uint32_t TRISE;
};

#define I2C1 ((struct i2c *)0x40005400)
#define I2C2 ((struct i2c *)0x40005800)

// =============================================================================
// =============================================================================
// =============================================================================

#define CHECK_ERROR_ENDTX(n)    if (n) { \
                                    end_i2c_tx(); \
                                    return FAILURE; \
                                }

#define CHECK_NULLPTR_ENDTX(n)  if (!n) { \
                                    end_i2c_tx(); \
                                    return FAILURE; \
                                }

void config_i2c(void);

// Use cases:
//      i2c_tx(ADDR, &SRC, N);
//
//      start_i2c_tx(ADDR);
//      i2c_tx(NO_COND, &SRC, N);
//      end_i2c_tx();
#define NO_COND -1
#define NO_STOP -2
uint32_t    *i2c_tx(int32_t addr, uint32_t *src, uint32_t nbytes);
uint32_t    start_i2c_tx(uint32_t addr);
void        end_i2c_tx(void);

uint32_t    *i2c_rx(uint32_t addr, uint32_t *dest, int32_t nbytes);

void        i2c_scan(void);

#endif

