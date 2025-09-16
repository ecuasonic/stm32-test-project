#include "core_stm/usart.h"
#include "core_stm/gpio.h"
#include "types.h"

// stty -F /dev/ttyUSB0 115200 cs8 -cstopb -parenb -crtscts raw -echo
//      Without "raw -echo", on "cat ...", rx = tx and breaks everything
// cat /dev/ttyUSB0
// echo "whatever" > /dev/ttyUSB0

void setup_usart(struct usart *usart) {
    usart->CR1 |= USART_CR1_UE;

    // 1 stop bit, 8N1
    usart->CR1 &= ~USART_CR1_M;
    usart->CR2 &= ~USART_CR2_STOP(0x3);

    // 115200 baud assuming 8 MHz PCLK (HSI)
    usart->BRR = USART_BRR_DIV_M(5) | USART_BRR_DIV_F(4);

    usart->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

char *usart_tx(struct usart *usart, char *src, uint32_t nbytes) {
    vuint32_t *sr = &usart->SR;
    vuint8_t *dr = (vuint8_t *)&usart->DR;

    while (*src && nbytes--) {
        while (!(*sr & USART_SR_TXE));
        *dr = *src++;
    }
    while (!(*sr & USART_SR_TC));

    return src;
}

char *usart_rx(struct usart *usart, char *dest, uint32_t nbytes) {
    vuint32_t *sr = &usart->SR;
    vuint8_t *dr = (vuint8_t *)&usart->DR;

    while (nbytes--) {
        while (!(*sr & USART_SR_RXNE));
        *dest = *dr;
        if (*dest++ == '\n')
            break;
    }
    *dest = '\0';

    return dest;
}
