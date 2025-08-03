#include "types.h"
#include "systick.h"

extern int main(void);

__attribute__((weak)) void Default_Handler(void) {
    while (1);
}
#define WEAK_ALIAS(x) __attribute__((weak, alias(#x)))

void NMI_Handler(void)          WEAK_ALIAS(Default_Handler);
void HardFault_Handler(void)    WEAK_ALIAS(Default_Handler);
void MemManage_Handler(void)    WEAK_ALIAS(Default_Handler);
void BusFault_Handler(void)     WEAK_ALIAS(Default_Handler);
void UsageFault_Handler(void)   WEAK_ALIAS(Default_Handler);
void SVC_Handler(void)          WEAK_ALIAS(Default_Handler);
void DebugMon_Handler(void)     WEAK_ALIAS(Default_Handler);
void PendSV_Handler(void)       WEAK_ALIAS(Default_Handler);

// startup code
__attribute__((naked, noreturn)) void _reset(void) {
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    // memset .bss to zero,
    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;

    // copy .data section to RAM region
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    systick_init(8000000 / 1000); // HSI (8 MHz)

    main();

    for (;;) (void) 0;
}

extern volatile uint32_t s_ticks; // Defined in systick.c
void SysTick_Handler(void) {
    s_ticks++;
}

extern void _estack(void); // Defined in linker.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors")))
void (*const tab[16+91])(void) = {
    _estack,
    _reset,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,

    // Fill all 91 IRQ slots with Default_Handler
    [16 ... 16+90] = Default_Handler
};
