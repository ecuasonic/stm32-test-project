#include "core_stm/dma.h"

// Figure out how to start DMA with I2C, then make DMA interrupt for full transfer.
// Make sure that two peripherals with same channel are not on at the same time.
void setup_channel(uint32_t c, uint32_t cpar, uint32_t cmar, uint32_t cndtr, uint16_t ccr) {
    struct dma *dma = (c < 8) ? DMA1 : DMA2;
    dma->C[c].CPAR = cpar;
    dma->C[c].CMAR = cmar;
    dma->C[c].CNDTR = cndtr;
    dma->C[c].CCR = ccr;
}
