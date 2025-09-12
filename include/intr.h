#ifndef INTR_H
#define INTR_H

#include "types.h"

#define IMR_bit     (1 << 0)
#define EMR_bit     (1 << 1)
#define RTSR_bit    (1 << 2)
#define FTSR_bit    (1 << 3)
// Usage: config_intr(0, 'B', IMR_bit | EMR_bit | RTSR_bit | FTSR_bit);
void config_intr(uint32_t exti, uint32_t port, uint32_t conf);

#endif
