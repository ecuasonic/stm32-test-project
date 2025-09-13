#ifndef VECTORS_H
#define VECTORS_H

__attribute__((noreturn))
void Reset_Handler(void);           // -
void SysTick_Handler(void);         // -
void RTC_Handler(void);             // 3
void EXTI0_Handler(void);           // 6
void EXTI1_Handler(void);           // 7
void DMA1_Channel4_Handler(void);   // 14
void DMA1_Channel6_Handler(void);   // 16

#endif
