#include "types.h"
#include "core_stm/exti.h"
#include "core_stm/gpio.h"
#include "cortex-m3/asm.h"
#include "cortex-m3/nvic/systick.h"
#include "core_stm/rtc.h"
#include "types.h"
#include "vectors.h"

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
    RTC_Handler,
    Default_Handler,    // FLASH
    Default_Handler,    // RCC
    EXTI0_Handler,
    EXTI1_Handler,
    Default_Handler,    // EXTI2
    Default_Handler,    // EXTI3
    Default_Handler,    // EXTI4
    Default_Handler,    // DMA1_Channel1
    Default_Handler,    // DMA1_Channel2
    Default_Handler,    // DMA1_Channel3
    DMA1_Channel4_Handler,
    Default_Handler,    // DMA1_Channel5
    DMA1_Channel6_Handler,
    Default_Handler,    // DMA1_Channel7
    Default_Handler,    // ADC1_2
    Default_Handler,    // USB_HP_CAN_TX
    Default_Handler,    // USB_HP_CAN_RX0
    Default_Handler,    // CAN_RX1
    Default_Handler,    // CAN_SCE
    Default_Handler,    // EXTI9_5
    Default_Handler,    // TIM1_BRK
    Default_Handler,    // TIM1_UP
    Default_Handler,    // TIM_TRG_COM
    Default_Handler,    // TIM1_CC
    Default_Handler,    // TIM2
    Default_Handler,    // TIM3
    Default_Handler,    // TIM4
    Default_Handler,    // I2C1_EV
    Default_Handler,    // I2C1_ER
    Default_Handler,    // I2C2_EV
    Default_Handler,    // I2C2_ER
    Default_Handler,    // SPI1
    Default_Handler,    // SPI2
    Default_Handler,    // USART1
    Default_Handler,    // USART2
    Default_Handler,    // USART3
    Default_Handler,    // EXTI5_10
    Default_Handler,    // RTCAlarm
    Default_Handler,    // USBWakeup
    Default_Handler,    // TIM8_BRK
    Default_Handler,    // TIM8_UP
    Default_Handler,    // TIM8_TRG_COM
    Default_Handler,    // TIM8_CC
    Default_Handler,    // ADC3
    Default_Handler,    // FSMC
    Default_Handler,    // SDIO
    Default_Handler,    // TIM5
    Default_Handler,    // SPI3
    Default_Handler,    // UART4
    Default_Handler,    // UART5
    Default_Handler,    // TIM6
    Default_Handler,    // TIM7
    Default_Handler,    // DMA2_Channel1
    Default_Handler,    // DMA2_Channel2
    Default_Handler,    // DMA2_Channel3
    Default_Handler,    // DMA2_Channel4_5
};
