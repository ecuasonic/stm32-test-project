#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

#include "types.h"

// Refer to Solomon Sytech SSD1306 datasheet
#define OLED_I2C_ADDR1 0x3C
#define OLED_I2C_ADDR2 0x3D

#define OLED_CO         (1 << 8)
#define OLED_D          (1 << 7)

#define OLED_DATA(n)    ((n)&0xFF)

// =============================================

#define OLED_COL    128
#define OLED_ROW32  32
#define OLED_ROW64  64
#define OLED_PAGE   8

#define OLED_PAGE32  4
#define OLED_PAGE64  8

struct oled {
    uint32_t cursor_x;
    uint32_t cursor_y;
    uint32_t rows;
    uint32_t addr;
    uint32_t configured;
    uint32_t on;
    struct i2c *i2c;
};

uint32_t config_oled(struct oled *oled, struct i2c *i2c, uint32_t addr, uint32_t rows);

uint32_t toggle_oled(struct oled *oled);
uint32_t send_cmd_oled(struct oled *oled, uint32_t *cmd, uint32_t n);
uint32_t send_data_oled(struct oled *oled, uint32_t *data, uint32_t n);

uint32_t set_oled_cursor(struct oled *oled, uint32_t x, uint32_t y);
uint32_t print_char_oled(struct oled *oled, uint32_t c);

uint32_t clear_oled(struct oled *oled);
uint32_t print_oled(struct oled *oled, char *str);
uint32_t print_image_oled(struct oled *oled, const uint32_t *image);

uint32_t config_scroll_oled(struct oled *oled, uint32_t start_page, uint32_t end_page);
uint32_t set_scroll_oled(struct oled *oled);
uint32_t unset_scroll_oled(struct oled *oled);

#endif
