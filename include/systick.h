#include "types.h"
#include "arm-cortex-m3/nvic/sys_timer.h"

INLINE void systick_init(uint32_t ticks) {
    SYSTICK->CTRL = SYSTICK_ENABLE | SYSTICK_TICKINT | SYSTICK_CLKSOURCE;
    SYSTICK->LOAD = SYSTICK_LOAD((ticks - 1) & SYSTICK_LOAD_MASK);
    SYSTICK->CURRENT = SYSTICK_CURRENT(0);
}

extern volatile uint32_t s_ticks;

void delay(unsigned ms);
uint32_t timer_expired(uint32_t *t, uint32_t prd, uint32_t now);
