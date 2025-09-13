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
    uint32_t full_display_on;
    uint32_t inverse_on;
    uint32_t on;
    uint32_t scroll_right;
    uint32_t scroll_down;
    uint32_t scroll_on;
    uint32_t pa_mem;
    struct i2c *i2c;
};

uint32_t config_oled(struct oled *oled, struct i2c *i2c, uint32_t addr, uint32_t rows);

uint32_t send_cmd_oled(struct oled *oled, uint32_t *cmd, uint32_t n);
uint32_t send_data_oled(struct oled *oled, uint32_t *data, uint32_t n);

uint32_t set_oled_cursor_pa(struct oled *oled, uint32_t x, uint32_t y);
uint32_t print_char_oled(struct oled *oled, uint32_t c);
uint32_t print_oled(struct oled *oled, char *str);

uint32_t clear_oled(struct oled *oled);
uint32_t print_image_oled(struct oled *oled, const uint32_t *image);
uint32_t print_image_oled_dma(struct oled *oled, const uint32_t *image);

// =============================================================================

INLINE uint32_t oled_cmd_toggle_full_display(struct oled *oled) {
    uint32_t cmd = (oled->full_display_on) ? 0xA4 : 0xA5;
    CHECK_ERROR(send_cmd_oled(oled, &cmd, 1));
    oled->full_display_on = !oled->full_display_on;

    return SUCCESS;
}

INLINE uint32_t oled_cmd_toggle_inverse(struct oled *oled) {
    uint32_t cmd = (oled->inverse_on) ? 0xA6 : 0xA7;
    CHECK_ERROR(send_cmd_oled(oled, &cmd, 1));
    oled->inverse_on = !oled->inverse_on;

    return SUCCESS;
}

INLINE uint32_t oled_cmd_toggle_display(struct oled *oled) {
    uint32_t cmd = (oled->on) ? 0xAE : 0xAF;
    CHECK_ERROR(send_cmd_oled(oled, &cmd, 1));
    oled->on = !oled->on;

    return SUCCESS;
}

// =============================================================================

INLINE uint32_t oled_cmd_setup_hort_scroll(struct oled *oled) {
    uint32_t cmd[7];
    cmd[0] = (oled->scroll_right) ? 0x27 : 0x26;
    cmd[1] = 0x00;
    cmd[2] = 0x00;  // start page address
    cmd[3] = 0x00;  // time interval between each scroll step
    cmd[4] = 0x07;  // end page address
    cmd[5] = 0x00;
    cmd[6] = 0xFF;
    CHECK_ERROR(send_cmd_oled(oled, cmd, 7));
    oled->scroll_right = !oled->scroll_right;

    return SUCCESS;
}

INLINE uint32_t oled_cmd_setup_vert_scroll(struct oled *oled) {
    uint32_t cmd[6];
    cmd[0] = (oled->scroll_down) ? 0x2A : 0x29;
    cmd[1] = 0x00;
    cmd[2] = 0x00;  // start page address
    cmd[3] = 0x00;  // time interval between each scroll step
    cmd[4] = 0x07;  // end page address
    cmd[5] = 0x01;  // vertical scrolling effect
    CHECK_ERROR(send_cmd_oled(oled, cmd, 6));
    oled->scroll_down = !oled->scroll_down;

    return SUCCESS;
}

INLINE uint32_t oled_cmd_toggle_scroll(struct oled *oled) {
    uint32_t cmd = (oled->scroll_on) ? 0x2E : 0x2F;
    CHECK_ERROR(send_cmd_oled(oled, &cmd, 1));
    oled->scroll_on = !oled->scroll_on;

    return SUCCESS;
}

// =============================================================================

// Set Lower Column Start Address for Page Addressing Mode
INLINE uint32_t oled_cmd_set_col_pa(struct oled *oled, uint32_t col) {
    if (oled->pa_mem) {
        uint32_t cmd[2];
        cmd[0] = 0x00 | col&0xF;
        cmd[1] = 0x10 | (col >> 4)&0xF;
        CHECK_ERROR(send_cmd_oled(oled, cmd, 2));

        return SUCCESS;
    }

    return FAILURE;
}

INLINE uint32_t oled_cmd_toggle_mem(struct oled *oled) {
    uint32_t cmd[2];
    cmd[0] = 0x20;
    cmd[1] = (oled->pa_mem) ? 0x00 : 0x02;
    oled->pa_mem = !oled->pa_mem;
    CHECK_ERROR(send_cmd_oled(oled, cmd, 2));

    return SUCCESS;
}

INLINE uint32_t oled_cmd_set_col(struct oled *oled, uint32_t start_col, uint32_t end_col) {
    if (!oled->pa_mem) {
        uint32_t cmd[3];
        cmd[0] = 0x21;
        cmd[1] = start_col&0x7F;
        cmd[2] = end_col&0x7F;
        CHECK_ERROR(send_cmd_oled(oled, cmd, 3));

        return SUCCESS;
    }

    return FAILURE;
}

INLINE uint32_t oled_cmd_set_page(struct oled *oled, uint32_t start_page, uint32_t end_page) {
    if (!oled->pa_mem) {
        uint32_t cmd[3];
        cmd[0] = 0x22;
        cmd[1] = start_page&0x07;
        cmd[2] = end_page&0x07;
        CHECK_ERROR(send_cmd_oled(oled, cmd, 3));

        return SUCCESS;
    }

    return FAILURE;
}

INLINE uint32_t oled_cmd_set_page_start_pa(struct oled *oled, uint32_t page) {
    if (oled->pa_mem) {
        uint32_t cmd = 0xB0 | (page&0x7);
        CHECK_ERROR(send_cmd_oled(oled, &cmd, 1));

        return SUCCESS;
    }

    return FAILURE;
}

// =============================================================================

#endif
