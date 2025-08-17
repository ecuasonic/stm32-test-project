#include "types.h"
#include "systick.h"

volatile uint32_t s_ticks;

/**
 * @brief First call sets intial timer and automatically resets periodically.
 *
 * @param t - Expiration time (set once and done).
 * @param prd - Period between each expiration.
 * @param now - Current ticks, can change externally.
 * @return - 1 if expired, 0 if not.
 */
uint32_t timer_expired(uint32_t *t, uint32_t prd, uint32_t now) {
    if (now + prd < *t) *t = 0;                         // Time wrapped? Reset timer
    if (*t == 0) *t = now + prd;                        // First poll? Set expiration
    if (*t > now) return 0;                             // Not expired yet, return false

    // If expired, automatically set next expiration time.
    *t = (now - *t) > prd ? now + prd : *t + prd;
    return 1;
}
