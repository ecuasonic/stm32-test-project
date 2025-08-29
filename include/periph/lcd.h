#ifndef LCD_H
#define LCD_H

// Refer to Sparkfun HD44780 datasheet
// Must use 4-bit mode for I2C interface (specifically for I2C module installed on LCD).

#include "types.h"

#define LCD_RS      (1U << 0)
#define LCD_RW      (1U << 1)
#define LCD_E       (1U << 2)
#define LCD_BL      (1U << 3)
#define LCD_DATA(n) (((n)&0xFU) << 4)

#define LCD_MAX_CHARS 20*4

#endif
