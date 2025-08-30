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
    //        ALERT (SMBus alert)
    //        PEC (Packet error checking)
    //        POS (ACK/PEC Position for data reception)
    //        ACK (ACK enable)
#define I2C_CR1_ACK     (1U << 10)
    //        STOP (Stop generation)
#define I2C_CR1_STOP    (1U << 9)
    //        START (Start generation)
#define I2C_CR1_START   (1U << 8)
    //        NOSTRETCH (Clock stretching disable [Slave mode])
    //        ENGC (General call enable)
    //        ENPEC (PEC enable)
    //        ENARP (ARP enable)
    //        SMBTYPE (SMBus type)
    //        SMBUS (SMBus mode)
    //        PE (Peripheral mode)
#define I2C_CR1_PE    (1U << 0)
    uint32_t CR1;

    // ==========================================
    // I2C Control Register 2 (I2C_CR2) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        LAST (DMA last transfer)
    //        DMAEN (DMA requests enable)
    //        ITBUFEN (Buffer interrupt enable)
    //        ITEVTEN (Event interupt enable)
    //        ITERREN (Error interrupt enable)
    //        FREQ (Peripheral clock frequency)
#define I2C_CR2_FREQ(n) (((n)&0x3FU) << 0)
    uint32_t CR2;

    // ==========================================
    // I2C Own Address Register 1 (I2C_OAR1) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        ADDMODE (Addressing mode [slave mode])
    //        ADD (Interface address)
    uint32_t OAR1;

    // ==========================================
    // I2C Own Address Register 2 (I2C_OAR2) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        ADD (Interface address)
    //        ENDUAL (Dual addressing mode enable)
    uint32_t OAR2;

    // ==========================================
    // I2C Data Register (I2C_DR) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        DR (8-bit data register)
    uint32_t DR;

    // ==========================================
    // I2C Status Register 1 (I2C_SR1) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        SMBALERT (SMBus Alert) (rc_w0)
    //        TIMEOUT (Timeout or Tlow error) (rc_w0)
    //        PECERR (PEC Error in reception) (rc_w0)
    //        OVR (Overrun/Underrun) (rc_w0)
    //        AF (Acknowledgement failure) (rc_w0)
    //        ARLO (Arbitration failure [master mode]) (rc_w0)
    //        BERR (Bus error) (rc_w0)
    //        TxE (Data register empty [transmitters]) (r)
#define I2C_SR1_TXE     (1U << 7)
    //        RxNE (Data register not empty [recievers]) (r)
#define I2C_SR1_RXNE    (1U << 6)
    //        STOPF (Stop detection [slave mode]) (r)
    //        ADD10 (10-bit header sent [Master mode]) (r)
    //        BTF (Byte transfer finished) (r)
    //        ADDR (Address sent [master mode] / Address matched [slave mode]) (r)
#define I2C_SR1_ADDR  (1U << 1)
    //        SB (Start bit [master mode]) (r)
#define I2C_SR1_SB    (1U << 0)
    uint32_t SR1;

    // ==========================================
    // I2C Status Register 2 (I2C_SR2) (r) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        PEC (Packet error checking register)
    //        DUALF (Dual flag [slave mode])
    //        SMBHOST (SMBus host header [slave mode])
    //        SMBDEFAULT (SMBus device default address [slave mode])
    //        GENCALL (General call address [slave mode])
    //        TRA (Transmitter/reciever)
    //        BUSY (Bus busy)
    //        MSL (Master/slave)
    uint32_t SR2;

    // ==========================================
    // I2C Clock Control Register (I2C_CCR) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        F/S (Master mode selection)
    //        DUTY (Fm Master mode duty cycle)
    //        CCR (Clock control register in Fm/Sm Master mode)
#define I2C_CCR(n)    (((n)&0xFFFU) << 0)
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

void config_i2c(void);

#define RX 1
#define TX 0
void start_comm(uint32_t addr, uint32_t rd);
void end_comm(void);
void tx(uint32_t byte);

#endif

