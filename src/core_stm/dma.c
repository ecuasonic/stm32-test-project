#include "core_stm/dma.h"
#include "core_stm/exti.h"
#include "core_stm/i2c.h"
#include "vectors.h"
#include "types.h"

// Figure out how to start DMA with I2C, then make DMA interrupt for full transfer.
// Make sure that two peripherals with same channel are not on at the same time.
void enable_dma_channel(uint32_t c, uint32_t *cpar, uint32_t *cmar, uint32_t cndtr, uint32_t ccr) {
    struct dma *dma = (c < 7) ? DMA1 : DMA2;
    if (dma->C[c].CCR & DMA_CCR_EN)
        panic();

    dma->C[c].CPAR = DMA_CPAR_PA((uint32_t)cpar);
    dma->C[c].CMAR = DMA_CMAR_MA((uint32_t)cmar);
    dma->C[c].CNDTR = DMA_CNDTR_NDT(cndtr);
    dma->C[c].CCR = ccr & 0x7FFF;
    dma->C[c].CCR |= DMA_CCR_EN;
}

void disable_dma_channel(uint32_t c) {
    struct dma *dma = (c < 7) ? DMA1 : DMA2;
    dma->C[c].CPAR = 0;
    dma->C[c].CMAR = 0;
    dma->C[c].CNDTR = 0;
    dma->C[c].CCR = 0;
    // Even through trc says CGIF clears all flags, lies.
    // Must clear here, if HTIF is set, then only half of next transfer will go through.
    dma->IFCR = DMA_IFCR_CTEIF(c) | DMA_IFCR_CHTIF(c) | DMA_IFCR_CTCIF(c) | DMA_IFCR_CGIF(c);
}

// =============================================================================

// Why does HTIF trigger TCIF?????
void DMA1_Channel6_Handler(void) {
    // if (DMA1->ISR & DMA_ISR_HTIF(6-1)) {
    //     DMA1->IFCR = DMA_IFCR_CHTIF(6-1);
    // }

    if (DMA1->ISR & DMA_ISR_TCIF(6-1)) {
        disable_dma_channel(6-1);

        vuint32_t *i2c_sr1 = &I2C1->SR1;
        if (I2C1->CR2 & I2C_CR2_DMAEN) {
            I2C1->CR2 &= ~I2C_CR2_DMAEN;

            i2c_sr1 = &I2C1->SR1;
            while (!(*i2c_sr1 & I2C_SR1_BTF));

            end_i2c_tx(I2C1);
        }
    }
}

void DMA1_Channel4_Handler(void) {
    if (DMA1->ISR & DMA_ISR_TCIF(4-1)) {
        DMA1->IFCR = DMA_IFCR_CTCIF(4-1);
        disable_dma_channel(4-1);

        vuint32_t *i2c_sr1 = &I2C2->SR1;
        if (I2C2->CR2 & I2C_CR2_DMAEN) {
            I2C2->CR2 &= ~I2C_CR2_DMAEN;

            i2c_sr1 = &I2C2->SR1;
            while (!(*i2c_sr1 & I2C_SR1_BTF));

            end_i2c_tx(I2C2);
        }
    }
}
