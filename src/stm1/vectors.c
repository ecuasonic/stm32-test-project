#include "types.h"
#include "core_stm/exti.h"
#include "core_stm/gpio.h"
#include "cortex-m3/nvic/systick.h"
#include "cortex-m3/asm.h"

extern int main(void);

__attribute__((weak)) void Default_Handler(void) {
    while ((volatile uint32_t)1);
}
// Redefinition will replace weak alias for function.
#define WEAK_ALIAS(x) __attribute__((weak, alias(#x)))

void NMI_Handler(void)              WEAK_ALIAS(Default_Handler);
void HardFault_Handler(void)        WEAK_ALIAS(Default_Handler);
void MemManage_Handler(void)        WEAK_ALIAS(Default_Handler);
void BusFault_Handler(void)         WEAK_ALIAS(Default_Handler);
void UsageFault_Handler(void)       WEAK_ALIAS(Default_Handler);
void SVCall_Handler(void)           WEAK_ALIAS(Default_Handler);
void DebugMonitor_Handler(void)     WEAK_ALIAS(Default_Handler);
void PendSV_Handler(void)           WEAK_ALIAS(Default_Handler);

// startup code
__attribute__((noreturn)) void Reset_Handler(void) {
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
static void SysTick_Handler(void) {
    s_ticks++;
}

// Puts CPU to sleep
vuint32_t sleep_request = 0;
static void EXTI0_Handler(void) {
    // while ((volatile uint32_t)1);
    if (EXTI->PR & EXTI_PR(0)) {
        EXTI->PR = EXTI_PR(0);

        gpio_clear('B', 10);
        sleep_request = 1;
    }
}

// Set as interrupt+event
// Wakes up CPU from sleep
static void EXTI1_Handler(void) {
    if (EXTI->PR & EXTI_PR(1)) {
        EXTI->PR = EXTI_PR(1);
        gpio_set('B', 10);
    }
}

extern void _estack(void); // Defined in linker.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors")))
void (*const tab[16+91])(void) = {
    _estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVCall_Handler,
    DebugMonitor_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,

    Default_Handler,    // WWDG
    Default_Handler,    // PVD
    Default_Handler,    // TAMPER
    Default_Handler,    // RTC
    Default_Handler,    // FLASH
    Default_Handler,    // RCC
    EXTI0_Handler,      // EXTI0
    EXTI1_Handler,      // EXTI1
    Default_Handler,
    Default_Handler,

    // Fill all 91 IRQ slots with Default_Handler
    [26 ... 26+80] = Default_Handler
};
