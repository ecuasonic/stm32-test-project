#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"
#include "types.h"
#include "periph/lcd.h"
#include "strings.h"

static const uint32_t lcd_line_addr[LCD_ROWS] = {0x00, 0x40, 0x14, 0x54};

// Print rough byte as is (use macros in lcd.h).
// Most basic form of sending information to LCD.
void tx_lcd(uint32_t byte) {
    tx(byte | LCD_E);
    tx(byte & ~LCD_E);
}

// Send data to print on screen.
// ex: 'A', 'B', etc.
void tx_lcd_data(uint32_t data) {
    tx_lcd(LCD_BL | LCD_RS | (data & 0xF0));
    tx_lcd(LCD_BL | LCD_RS | ((data & 0x0F) << 4));
}

// Send instructions
// Look at trm of HD44780 for instructions.
void tx_lcd_inst(uint32_t inst) {
    tx_lcd(LCD_BL | (inst & 0xF0));
    tx_lcd(LCD_BL | ((inst & 0x0F) << 4));
}

// Send string up to n characters.
static void tx_lcd_nstr(char *str, uint32_t n) {
    uint32_t stop = 0;
    while (n-- && *str) {
        if (cursor_x >= LCD_COLS)
            stop = 1;
        if (*str == '\n' || stop) {
            cursor_y = (cursor_y + 1) % LCD_ROWS;
            tx_lcd_inst(0x80 | lcd_line_addr[cursor_y]);
            cursor_x = 0;
            if (stop) {
                return;
            }
        } else {
            tx_lcd_data(*str);
            cursor_x++;
        }
        str++;
    }
}

// ===================================================================

void config_lcd(void) {
    delay(5);
    start_comm(LCD_I2C_ADDR, TX);
    tx_lcd(LCD_DATA(0x2));      // Set to 4-bit operation
    tx_lcd_inst(0x2C);          // 2 lines, 5x10 dots
    tx_lcd_inst(0x0E);          // Display on, cursor on, blinking
    tx_lcd_inst(0x01);          // Clear screen
    end_comm();
    delay(5);
}

// ===================================================================

void print_lcd(char *str) {
    start_comm(LCD_I2C_ADDR, TX);
    tx_lcd_nstr(str, LCD_TOT);
    end_comm();
}

void clear_lcd(void) {
    cursor_y = 0;
    cursor_x = 0;
    start_comm(LCD_I2C_ADDR, TX);
    tx_lcd_inst(0x01);
    end_comm();
    delay(2);
}

void move_lcd_cursor(int32_t dx, int32_t dy, int32_t rel) {
    if (rel) {
        cursor_y += (uint32_t)dy;
        cursor_x += (uint32_t)dx;
    } else {
        cursor_y = (uint32_t)dy;
        cursor_y = (uint32_t)dx;
    }
    cursor_y %= LCD_ROWS;
    cursor_x %= LCD_COLS;

    start_comm(LCD_I2C_ADDR, TX);
    tx_lcd_inst(0x80 | lcd_line_addr[cursor_y] + cursor_x);
    end_comm();
}

void home_lcd_cursor(void) {
    cursor_y = 0;
    cursor_x = 0;
    start_comm(LCD_I2C_ADDR, TX);
    tx_lcd_inst(0x2);
    end_comm();
}

// ===================================================================

void repl_str_lcd(char *old, char *new, uint32_t n) {
    uint32_t old_len = strnlen(old, n);
    char *new_str = strndiff(old, new, n);
    int32_t dx = new_str - new;

    move_lcd_cursor((int32_t)((uint32_t)dx - old_len), 0, REL);
    print_lcd(new_str);
}

// ===================================================================

void counter_lcd(uint32_t max) {
    int32_t n = 0;
    char old[LCD_COLS+1];
    char new[LCD_COLS+1];

    itoa(n, 10, old, LCD_COLS+1);
    print_lcd(old);
    max++;
    while(max--) {
        itoa(n++, 10, new, LCD_COLS+1);
        repl_str_lcd(old, new, 20);
        strncpy(old, new, LCD_COLS+1);
        delay(9);
    }
}

