#ifndef USART_H
#define USART_H

#include "types.h"

struct usart {
    // ==========================================
    // Status Register (USART_SR)
    //    Reset value:
    //        0x00C0
    //    Bits:
    //        CTS (CTS flag) (rc_w0) (0)
#define USART_SR_CTS  (1U << 9)
    //        LDB (LIN break detection flag) (rc_w0) (0)
#define USART_SR_LDB  (1U << 8)
    //        TXE (Transmit data register empty) (r) (1)
#define USART_SR_TXE  (1U << 7)
    //        TC (Transmission complete) (rc_w0) (1)
#define USART_SR_TC   (1U << 6)
    //        RXNE (Read data register not empty) (rc_w0) (0)
#define USART_SR_RXNE (1U << 5)
    //        IDLE (IDLE line detected) (r) (0)
#define USART_SR_IDLE (1U << 4)
    //        ORE (Overrun error) (r) (0)
#define USART_SR_ORE  (1U << 3)
    //        NE (Noise error flag) (r) (0)
#define USART_SR_NE   (1U << 2)
    //        FE (Framing error) (r) (0)
#define USART_SR_FE   (1U << 1)
    //        PE (Parity error) (r) (0)
#define USART_SR_PE   (1U << 0)
    uint32_t SR;

    // ==========================================
    // Data Register (USART_DR) (rw) (0)
    //    Reset value:
    //        Undefined
    //    Bits:
    //        DR (Data value)
#define USART_DR(n)   ((n)&0x1FFU)
    uint32_t DR;

    // ==========================================
    // Baud rate register (USART_BRR) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        DIV_Mantissa (Mantissa of USARTDIV)
#define USART_BRR_DIV_M(n)    ((n)&0xFU)
    //        DIV_Fraction (Fraction of USARTDIV)
#define USART_BRR_DIV_F(n)    (((n)&0xFFFU) << 4)
    uint32_t BRR;

    // ==========================================
    // Control register 1 (USART_CR1) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        UE (UART enable)
#define USART_CR1_UE      (1U << 13)
    //        M (Word length)
#define USART_CR1_M       (1U << 12)
    //        WAKE (Wakeup method)
#define USART_CR1_WAKE    (1U << 11)
    //        PCE (Parity control enable)
#define USART_CR1_PCE     (1U << 10)
    //        PS (Parity selection)
#define USART_CR1_PS      (1U << 9)
    //        PEIE (PE interrupt enable)
#define USART_CR1_PEIE    (1U << 8)
    //        TXEIE (TXE interrupt enable)
#define USART_CR1_TXEIE   (1U << 7)
    //        TCIE (Transmission complete interrupt enable)
#define USART_CR1_TCIE    (1U << 6)
    //        RXNEIE (RXNE interrupt enable)
#define USART_CR1_RXNEIE  (1U << 5)
    //        IDLEIE (IDLE interrupt enable)
#define USART_CR1_IDLEIE  (1U << 4)
    //        TE (Transmitter enable)
#define USART_CR1_TE      (1U << 3)
    //        RE (Receiver enable)
#define USART_CR1_RE      (1U << 2)
    //        RWU (Receiver wakeup)
#define USART_CR1_RWU     (1U << 1)
    //        SBK (Send break)
#define USART_CR1_SBK     (1U << 0)
    uint32_t CR1;

    // ==========================================
    // Control register 2 (USART_CR2) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        LINEN (LIN mode enable)
#define USART_CR2_LINEN   (1U << 14)
    //        STOP (STOP bits)
#define USART_CR2_STOP(n) (((n)&0x3U) << 12)
    //        CLKEN (Clock enable)
#define USART_CR2_CLKEN   (1U << 11)
    //        CPOL (Clock polarity)
#define USART_CR2_CPOL    (1U << 10)
    //        CPHA (Clock phase)
#define USART_CR2_CPHA    (1U << 9)
    //        LBCL (Last bit clock phase)
#define USART_CR2_LBCL    (1U << 8)
    //        LBDIE (LIN break detection interrupt enable)
#define USART_CR2_LBDIE   (1U << 6)
    //        LBDL (LIN break detection length)
#define USART_CR2_LBDL    (1U << 5)
    //        ADD (Address of the USART node)
#define USART_CR2_ADD(n)  ((n)&0xFU)
    uint32_t CR2;

    // ==========================================
    // Control register 3 (USART_CR3) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        CTSIE (CTS interrupt enable)
#define USART_CR3_CTSIE    (1U << 10)
    //        CTSE (CTS enable)
#define USART_CR3_CTSE     (1U << 9)
    //        RTSE (RTS enable)
#define USART_CR3_RTSE     (1U << 8)
    //        DMAT (DMA enable transmitter)
#define USART_CR3_DMAT     (1U << 7)
    //        DMAR (DMA enable receiver)
#define USART_CR3_DMAR     (1U << 6)
    //        SCEN (Smartcard mode enable)
#define USART_CR3_SCEN     (1U << 5)
    //        NACK (Smartcard NACK enable)
#define USART_CR3_NACK     (1U << 4)
    //        HDSEL (Half-duplex selection)
#define USART_CR3_HDSEL    (1U << 3)
    //        IRLP (IrDA mode enable)
#define USART_CR3_IRLP     (1U << 2)
    //        IREN (IrDA mode enable)
#define USART_CR3_IREN     (1U << 1)
    //        EIE (Error interrupt enable)
#define USART_CR3_EIE      (1U << 0)
    uint32_t CR3;

    // ==========================================
    // Guard time and prescalar register (USART_GTPR) (rw) (0)
    //    Reset value:
    //        0x0000
    //    Bits:
    //        GT (Guard time value)
#define USART_GTPR_GT(n)  (((n)&0xFF) << 8)
    //        PSC (Prescalar value)
#define USART_GTPR_PSC(n) ((n)&0xFF)
    uint32_t GTPR;
};

#define USART1 ((struct usart *)0x40013800)
#define USART2 ((struct usart *)0x40004400)
#define USART3 ((struct usart *)0x40004800)
#define USART4 ((struct usart *)0x40004C00)
#define USART5 ((struct usart *)0x40005000)

#define CONSOLE_LENGTH 100
void setup_usart(struct usart *usart);
char *usart_tx(struct usart *usart, char *src, uint32_t nbytes);
char *usart_rx(struct usart *usart, char *dest, uint32_t nbytes);

#endif

