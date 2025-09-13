#include "intr.h"
#include "core_stm/exti.h"
#include "core_stm/afio.h"
#include "defines.h"
#include "types.h"

void config_intr(uint32_t exti, uint32_t port, uint32_t conf) {
    vuint32_t *exticr1 = &AFIO->EXTICR1;

    *exticr1 &= CLEAR_EXTICR(exti);
    *exticr1 |= AFIO_EXTICR_EXTI(port, exti);

    EXTI->PR = EXTI_MR(exti);

    if (conf & IMR_bit)
        EXTI->IMR |= EXTI_MR(exti);

    if (conf & EMR_bit)
        EXTI->EMR |= EXTI_MR(exti);

    if (conf & RTSR_bit)
        EXTI->RTSR |= EXTI_TR(exti);

    if (conf & FTSR_bit)
        EXTI->FTSR |= EXTI_TR(exti);
}

__attribute__((noreturn)) void panic(void) {
    for(;;);
}
