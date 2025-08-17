### Overview

## RCC

***Must configure clocks then enable them.***

Don't forget volatile when checking for bits set by hardware.

Refer to Clock Tree in technical reference manual for HCLK, SYSCLK, PLL.

Has code to enable HSE+LSI and wait for it to be ready (HSE is not installed on board).

Set up PLL and connect to MCO to output on pin. MCO output interfers with pins on same port, so turn it off when not testing MCO.


## GPIO

***Must enable clocks then configure gpio registers***

Has code to enable Ports used by pins.

Refer to GPIO input/output types in technical reference manual.

Alternate function io used to output MCO and for EXTI0 input.


## SYSTICK

1 ms ticks (calculated using the Clock Tree).

SysTick interrupt increments global tick, which is used by main().

Refer to Cortex-m3 reference manual.

## INTERRUPTS

SysTick and EXTI0.

EXTI0 uses Pin A0 for external hardware interrupt, which triggers ISR that toggles Pin B0.

Can set trigger for risng and/or falling signal edge to Pin A0.

Refer to Cortex-M3 for enabling NVIC IRQ for EXTI.