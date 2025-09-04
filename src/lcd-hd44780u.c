#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"
#include "types.h"
#include "periph/lcd-hd44780u.h"
#include "strings.h"

static const uint32_t lcd_line_addr[LCD_ROWS] = {0x00, 0x40, 0x14, 0x54};
uint32_t lcd_configured;

// Print rough byte as is (use macros in lcd.h).
// Most basic form of sending information to LCD.
uint32_t tx_lcd(uint32_t byte) {
    byte |= LCD_E;
    CHECK_NULLPTR(i2c_tx(NO_COND, &byte, 1));
    byte &= ~LCD_E;
    CHECK_NULLPTR(i2c_tx(NO_COND, &byte, 1));

    return SUCCESS;
}

// Send data to print on screen.
// ex: 'A', 'B', etc.
uint32_t tx_lcd_data(uint32_t data) {
    CHECK_ERROR(tx_lcd(LCD_BL | LCD_RS | (data & 0xF0)));
    CHECK_ERROR(tx_lcd(LCD_BL | LCD_RS | ((data & 0x0F) << 4)));

    return SUCCESS;
}

// Send instructions
// Look at trm of HD44780 for instructions.
uint32_t tx_lcd_inst(uint32_t inst) {
    CHECK_ERROR(tx_lcd(LCD_BL | (inst & 0xF0)));
    CHECK_ERROR(tx_lcd(LCD_BL | ((inst & 0x0F) << 4)));

    return SUCCESS;
}

// Send string up to n characters.
static uint32_t tx_lcd_nstr(struct lcd *lcd, char *str, uint32_t n) {
    uint32_t stop = 0;
    while (n-- && *str) {
        if (lcd->cursor_x >= LCD_COLS)
            stop = 1;
        if (*str == '\n' || stop) {
            lcd->cursor_y = (lcd->cursor_y + 1) % LCD_ROWS;
            CHECK_ERROR(tx_lcd_inst(0x80 | lcd_line_addr[lcd->cursor_y]));
            lcd->cursor_x = 0;
            if (stop) {
                return SUCCESS;
            }
        } else {
            CHECK_ERROR(tx_lcd_data(*str));
            lcd->cursor_x++;
        }
        str++;
    }

    return SUCCESS;
}

// ===================================================================

uint32_t config_lcd(struct lcd *lcd, uint32_t addr) {
    delay(4);
    lcd->configured = 0;
    start_i2c_tx(addr);
    CHECK_ERROR_ENDTX(tx_lcd(LCD_DATA(0x2)));   // Set to 4-bit operation
    CHECK_ERROR_ENDTX(tx_lcd_inst(0x2C));       // 2 lines, 5x10 dots
    CHECK_ERROR_ENDTX(tx_lcd_inst(0x0C));       // Display on, cursor off, blinking
    CHECK_ERROR_ENDTX(tx_lcd_inst(0x01));       // Clear screen
    end_i2c_tx();
    delay(4);

    lcd->cursor_x = 0;
    lcd->cursor_y = 0;
    lcd->addr = addr;
    lcd->configured = 1;

    return SUCCESS;
}

// ===================================================================

uint32_t print_lcd(struct lcd *lcd, char *str) {
    start_i2c_tx(lcd->addr);
    CHECK_ERROR_ENDTX(tx_lcd_nstr(lcd, str, LCD_TOT));
    end_i2c_tx();

    return SUCCESS;
}

uint32_t print_num_lcd(struct lcd *lcd, int32_t num, uint32_t base) {
    static char str[LCD_COLS/2];
    itoa(num, base, str, LCD_COLS/2);
    CHECK_ERROR(print_lcd(lcd, str));

    return SUCCESS;
}

uint32_t clear_lcd(struct lcd *lcd) {
    lcd->cursor_y = 0;
    lcd->cursor_x = 0;
    start_i2c_tx(lcd->addr);
    CHECK_ERROR_ENDTX(tx_lcd_inst(0x01));
    end_i2c_tx();
    delay(2);

    return SUCCESS;
}

uint32_t set_lcd_cursor(struct lcd *lcd, uint32_t dx, uint32_t dy) {
    lcd->cursor_y = dy % LCD_ROWS;
    lcd->cursor_x = dx % LCD_COLS;

    start_i2c_tx(LCD_I2C_ADDR);
    CHECK_ERROR_ENDTX(tx_lcd_inst(0x80 | lcd_line_addr[lcd->cursor_y] + lcd->cursor_x));
    end_i2c_tx();

    return SUCCESS;
}

uint32_t home_lcd_cursor(struct lcd *lcd) {
    lcd->cursor_y = 0;
    lcd->cursor_x = 0;
    start_i2c_tx(LCD_I2C_ADDR);
    CHECK_ERROR_ENDTX(tx_lcd_inst(0x2));
    end_i2c_tx();

    return SUCCESS;
}

// ===================================================================

uint32_t repl_str_lcd(struct lcd *lcd, char *old, char *new, uint32_t n) {
    uint32_t old_len = strnlen(old, n);
    char *new_str = strndiff(old, new, n);
    int32_t dx = new_str - new;

    CHECK_ERROR(set_lcd_cursor(lcd, lcd->cursor_x + dx - old_len, lcd->cursor_y));
    CHECK_ERROR(print_lcd(lcd, new_str));

    return SUCCESS;
}

// ===================================================================

uint32_t counter_lcd(struct lcd *lcd, uint32_t max) {
    int32_t n = 0;
    char old[LCD_COLS+1];
    char new[LCD_COLS+1];

    itoa(n, 10, old, LCD_COLS+1);
    CHECK_ERROR(print_lcd(lcd, old));
    max++;
    while(max--) {
        itoa(n++, 10, new, LCD_COLS+1);
        CHECK_ERROR(repl_str_lcd(lcd, old, new, 20));
        strncpy(old, new, LCD_COLS+1);
        delay(9);
    }

    return SUCCESS;
}

// ===================================================================

#include "periph/acc.h"
void print_acc_data_lcd(struct lcd *lcd, char port, uint32_t pin, uint32_t n) {
    int32_t offset[6], raw[6], formatted[3];

    if (!acc_configured)
        return;

    get_single_data_acc(port, pin, offset);

    while (n--) {
        print_lcd(lcd, "ACC data...\n");

        get_single_data_acc(port, pin, raw);
        format_data_acc(raw, offset, formatted);

        print_lcd(lcd, "x = ");
        print_num_lcd(lcd, formatted[0], 10);
        print_lcd(lcd, "\n");

        print_lcd(lcd, "y = ");
        print_num_lcd(lcd, formatted[1], 10);
        print_lcd(lcd, "\n");

        print_lcd(lcd, "z = ");
        print_num_lcd(lcd, formatted[2], 10);
        print_lcd(lcd, "\n");

        delay(1000);
        clear_lcd(lcd);
    }
}

void print_acc_test_lcd(struct lcd *lcd, char port, uint32_t pin) {
    int32_t raw[6], formatted[3];

    if (!acc_configured)
        return;

    print_lcd(lcd, "ACC self-test...\n");

    get_test_acc(port, pin, raw);
    format_data_acc(raw, 0, formatted);

    print_lcd(lcd, "x = ");
    print_num_lcd(lcd, formatted[0], 10);
    if (-100 <= formatted[0] && formatted[0] <= 100) {
        print_lcd(lcd, " [GOOD]\n");
    } else {
        print_lcd(lcd, " [BAD]\n");
    }

    print_lcd(lcd, "y = ");
    print_num_lcd(lcd, formatted[1], 10);
    if (-100 <= formatted[1] && formatted[1] <= 100) {
        print_lcd(lcd, " [GOOD]\n");
    } else {
        print_lcd(lcd, " [BAD]\n");
    }

    print_lcd(lcd, "z = ");
    print_num_lcd(lcd, formatted[2], 10);
    if (-1000 <= formatted[2] && formatted[2] <= -300) {
        print_lcd(lcd, " [GOOD]\n");
    } else {
        print_lcd(lcd, " [BAD]\n");
    }
}
