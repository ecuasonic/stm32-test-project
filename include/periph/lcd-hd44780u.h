#ifndef LCD_HD44780U_H
#define LCD_HD44780U_H

// Refer to Sparkfun HD44780 datasheet
// Must use 4-bit mode for I2C interface (specifically for I2C module installed on LCD).
#define LCD_I2C_ADDR 0x27

#include "types.h"
#include "periph/acc.h"

#define LCD_RS      (1U << 0)
#define LCD_RW      (1U << 1)
#define LCD_E       (1U << 2)
#define LCD_BL      (1U << 3)
#define LCD_DATA(n) (((n)&0xFU) << 4)

#define LCD_COLS    20
#define LCD_ROWS    4
#define LCD_TOT     LCD_COLS * LCD_ROWS

struct lcd {
    uint32_t cursor_x;
    uint32_t cursor_y;
    uint32_t addr;
    uint32_t configured;
    struct i2c *i2c;
};

// =============================================================================
// =============================================================================
// =============================================================================

uint32_t print_lcd(struct lcd *lcd, char *str);
uint32_t print_num_lcd(struct lcd *lcd, int32_t num, uint32_t base);
uint32_t clear_lcd(struct lcd *lcd);
uint32_t repl_str_lcd(struct lcd *lcd, char *old, char *new, uint32_t n);

uint32_t config_lcd(struct lcd *lcd, struct i2c *i2c, uint32_t addr);

// Usage:
//      start_i2c_tx(lcd->addr);
//      tx_lcd(byte);
//      end_i2c_tx();
//
//      start_i2c_tx(lcd->addr);
//      tx_lcd_data(lcd, data);
//      end_i2c_tx();
//
//      start_i2c_tx(lcd->addr);
//      tx_lcd_inst(lcd, data);
//      end_i2c_tx();
uint32_t tx_lcd(struct lcd *lcd, uint32_t byte);
uint32_t tx_lcd_data(struct lcd *lcd, uint32_t data);
uint32_t tx_lcd_inst(struct lcd *lcd, uint32_t inst);

uint32_t set_lcd_cursor(struct lcd *lcd, uint32_t dx, uint32_t dy);
uint32_t home_lcd_cursor(struct lcd *lcd);

uint32_t counter_lcd(struct lcd *lcd, uint32_t max);

// =============================================================================

void print_acc_data_lcd(struct lcd *lcd, struct acc *acc, char port, uint32_t pin, uint32_t n);
void print_acc_test_lcd(struct lcd *lcd, struct acc *acc, char port, uint32_t pin);

#endif
