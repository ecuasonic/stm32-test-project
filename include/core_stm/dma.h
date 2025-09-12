#ifndef DMA_H
#define DMA_H

#include "types.h"

struct channel {
    // ==========================================
    // DMA Channel x Configuration Register (DMA_CCRx) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        MEM2MEM (Memory to memory mode)
#define DMA_CCR_MEM2MEM   (1U << 14)
    //        PL (Channel priority level)
#define DMA_CCR_PL(n)     (((n)&0x3U) << 12)
    //        MSIZE (Memory size)
#define DMA_CCR_MSIZE(n)  (((n)&0x3U) << 10)
    //        PSIZE (Peripheral size)
#define DMA_CCR_PSIZE(n)  (((n)&0x3U) << 8)
    //        MINC (Memory increment mode)
#define DMA_CCR_MINC      (1U << 7)
    //        PINC (Peripheral increment mode)
#define DMA_CCR_PINC      (1U << 6)
    //        CIRC (Circular mode)
#define DMA_CCR_CIRC      (1U << 5)
    //        DIR (Data transfer direction)
#define DMA_CCR_DIR       (1U << 4)
    //        TEIE (Transfer error interrupt enable)
#define DMA_CCR_TEIE      (1U << 3)
    //        HTIE (Half transfer interrupt enable)
#define DMA_CCR_HTIE      (1U << 2)
    //        TCIE (Transfer complete interrupt enable)
#define DMA_CCR_TCIE      (1U << 1)
    //        EN (Channel enable)
#define DMA_CCR_EN        (1U << 0)
    uint32_t CCR;

    // ==========================================
    // DMA Channel x (Number of Data) Register (DMA_CNDTRx) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        NDT (Number of data to transfer)
#define DMA_CNDTR_NDT(n)  ((n)&0xFFFFU)
    uint32_t CNDTR;

    // ==========================================
    // DMA Channel x Peripheral Address Register (DMA_CPARx) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        PA (Peripheral address)
#define DMA_CPAR_PA(n)    ((n)&0xFFFFFFFFU)
    uint32_t CPAR;

    // ==========================================
    // DMA Channel x Memory Address Register (DMA_CMARx) (rw) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        MA (Memory address)
#define DMA_CMAR_MA(n)    ((n)&0xFFFFFFFFU)
    uint32_t CMAR;
    uint32_t reserved;
};

struct dma {
    // ==========================================
    // DMA Interrupt Status Register (DMA_ISR) (r) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        TEIFx (Channel x transfer error flag) (x=1..7)
#define DMA_ISR_TEIF(x)   (1 << ((x)*4))
    //        HEIFx (Channel x half transfer flag) (x=1..7)
#define DMA_ISR_HTIF(x)   (1 << (((x)*4)+1))
    //        TCIFx (Channel x transfer complete flag) (x=1..7)
#define DMA_ISR_TCIF(x)   (1 << (((x)*4)+2))
    //        GIFx (Channel x global interrupt flag) (x=1..7)
#define DMA_ISR_GIF(x)    (1 << (((x)*4)+3))
    uint32_t ISR;

    // ==========================================
    // DMA Interrupt Flag Clear Register (DMA_IFCR) (w) (0)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
    //        CTEIFx (Channel x transfer error clear) (x=1..7)
#define DMA_ISR_CTEIF(x)   (1 << ((x)*4))
    //        CHEIFx (Channel x half transfer clear) (x=1..7)
#define DMA_ISR_CHTIF(x)   (1 << (((x)*4)+1))
    //        CTCIFx (Channel x transfer complete clear) (x=1..7)
#define DMA_ISR_CTCIF(x)   (1 << (((x)*4)+2))
    //        CGIFx (Channel x global interrupt clear) (x=1..7)
#define DMA_ISR_CGIF(x)    (1 << (((x)*4)+3))
    uint32_t IFCR;
    struct channel C[7];
};

#define DMA1 ((struct dma *)0x40020000)
#define DMA2 ((struct dma *)0x40020400)

void setup_channel(uint32_t c, uint32_t cpar, uint32_t cmar, uint32_t cndtr, uint16_t ccr);

#endif
