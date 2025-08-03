#include "types.h"
#include "systick.h"

volatile uint32_t s_ticks;

void delay(unsigned ms) {
    uint32_t until = s_ticks + ms;
    while (s_ticks < until) (void) 0;
}

uint32_t timer_expired(uint32_t *t, uint32_t prd, uint32_t now) {
    if (now + prd < *t) *t = 0;                         // Time wrapped? Reset timer
    if (*t == 0) *t = now + prd;                        // First poll? Set expiration
    if (*t > now) return 0;                             // Not expired yet, return false
    *t = (now - *t) > prd ? now + prd : *t + prd;       // Next expiration time
    return 1;                                           // Expired, return true
}
