#ifndef LCD_HD44780U_H
#define LCD_HD44780U_H

// Refer to Sparkfun HD44780 datasheet
// Must use 4-bit mode for I2C interface (specifically for I2C module installed on LCD).
#define LCD_I2C_ADDR 0x27

#include "types.h"

#define LCD_RS      (1U << 0)
#define LCD_RW      (1U << 1)
#define LCD_E       (1U << 2)
#define LCD_BL      (1U << 3)
#define LCD_DATA(n) (((n)&0xFU) << 4)

#define LCD_COLS    20
#define LCD_ROWS    4
#define LCD_TOT     LCD_COLS * LCD_ROWS

extern uint32_t cursor_x;
extern uint32_t cursor_y;

// =============================================================================
// =============================================================================
// =============================================================================

void print_lcd(char *str);
void print_num_lcd(int32_t num, uint32_t base);
void clear_lcd(void);
void repl_str_lcd(char *old, char *new, uint32_t n);

#define REL 1
#define ABS 0
void config_lcd(void);

void tx_lcd(uint32_t byte);
void tx_lcd_data(uint32_t data);
void tx_lcd_inst(uint32_t inst);

void set_lcd_cursor(int32_t dx, int32_t dy, int32_t rel);
void home_lcd_cursor(void);

void counter_lcd(uint32_t max);

#endif
