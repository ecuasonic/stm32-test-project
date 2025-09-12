#include "periph/oled-ssd1306.h"
#include "types.h"
#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"
#include "font.h"
#include "oled_images/flower.h"

#include "core_stm/gpio.h"

#define TRIES 150
uint32_t config_oled(struct oled *oled, struct i2c *i2c, uint32_t addr, uint32_t rows) {
    vint32_t tries = TRIES;
    while (tries-- && start_i2c_tx(i2c, addr));

    uint32_t src[8];

    oled->configured = 0;
    oled->on = 0;

    if (tries < 0) {
        return FAILURE;
    } else {
        end_i2c_tx(i2c);
        oled->addr = addr;
        oled->rows = rows;
        oled->cursor_x = 0;
        oled->cursor_y = 0;
        oled->i2c = i2c;

        if (rows == OLED_ROW32) {
            src[0] = OLED_DATA(0xA8);
            src[1] = OLED_DATA(0x1F); // 31
            CHECK_ERROR(send_cmd_oled(oled, src, 2));
        }

        // Charge pump ON (MUST)
        src[0] = OLED_DATA(0x8D);
        src[1] = OLED_DATA(0x14);
        CHECK_ERROR(send_cmd_oled(oled, src, 2));

        // Display ON
        src[0] = OLED_DATA(0xAF);
        CHECK_ERROR(send_cmd_oled(oled, src, 1));
        oled->on = 1;

        CHECK_ERROR(clear_oled(oled));

        oled->configured = 1;
        return SUCCESS;
    }
}

uint32_t send_cmd_oled(struct oled *oled, uint32_t *cmd, uint32_t n) {
    uint32_t start_cmd = 0x00;

    start_i2c_tx(oled->i2c, oled->addr);
    CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, &start_cmd, 1), oled->i2c);
    CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, cmd, n), oled->i2c);
    end_i2c_tx(oled->i2c);

    return SUCCESS;
}

uint32_t send_data_oled(struct oled* oled, uint32_t *data, uint32_t n) {
    uint32_t start_data = 0x40;

    start_i2c_tx(oled->i2c, oled->addr);
    CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, &start_data, 1), oled->i2c);
    CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, data, n), oled->i2c);
    end_i2c_tx(oled->i2c);

    return SUCCESS;
}

uint32_t set_oled_cursor(struct oled *oled, uint32_t x, uint32_t y) {
    uint32_t cmd;

    if (x >= OLED_COL) {
        return FAILURE;
    }

    cmd = 0xB0 | (y&0x7);
    CHECK_ERROR(send_cmd_oled(oled, &cmd, 1));

    cmd = 0x00 | (x&0xF);
    CHECK_ERROR(send_cmd_oled(oled, &cmd, 1));

    cmd = 0x10 | ((x >> 4)&0xF);
    CHECK_ERROR(send_cmd_oled(oled, &cmd, 1));

    oled->cursor_x = x&0xFF;
    oled->cursor_y = y&0x7;

    return SUCCESS;
}

uint32_t clear_oled(struct oled *oled) {
    uint32_t start_data = 0x40;
    uint32_t data = 0;

    toggle_oled(oled);
    for (uint32_t y = 0; y < OLED_PAGE; y++) {
        start_i2c_tx(oled->i2c, oled->addr);
        CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, &start_data, 1), oled->i2c);
        for (uint32_t x = 0; x < OLED_COL; x++) {
            CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, &data, 1), oled->i2c);
        }
        end_i2c_tx(oled->i2c);

        CHECK_ERROR(set_oled_cursor(oled, 0, y+1));
    }
    toggle_oled(oled);

    return SUCCESS;
}

uint32_t print_char_oled(struct oled *oled, uint32_t c) {
    uint32_t data, data4, proc;

    for (uint32_t col = 0; col < 6; col++) {
        data = font5x7[c - 0x20][col];
        if (oled->rows == OLED_ROW32) {
            // 32 rows only sees odd bits of data byte.
            // 4 bits per page

            // Lower nibble (0x X D3 X D2 X D1 X D0) on page n
            data4 = data&0xF;
            proc = 0;
            for (uint32_t i = 0; i < 4; i++) {
                proc |= ((data4 >> i)&0x1) << i*2;
            }
            CHECK_ERROR(send_data_oled(oled, &proc, 1));
            set_oled_cursor(oled, oled->cursor_x, oled->cursor_y+1);

            // Higher nibble (0x X D7 X D6 X D5 X D4) on page n+1
            data4 = (data >> 4)&0xF;
            proc = 0;
            for (uint32_t i = 0; i < 4; i++) {
                proc |= ((data4 >> i)&0x1) << i*2;
            }
            CHECK_ERROR(send_data_oled(oled, &proc, 1));
            set_oled_cursor(oled, oled->cursor_x+1, oled->cursor_y-1);
        } else if (oled->rows == 64){
            CHECK_ERROR(send_data_oled(oled, &data, 1));
            set_oled_cursor(oled, oled->cursor_x+1, oled->cursor_y);
        }
    }
    return SUCCESS;
}

// Similar to tx_lcd_nstr, might combine both and add general function to i2c.c
static uint32_t tx_oled_nstr(struct oled *oled,char *str, uint32_t n) {
    uint32_t dy = (oled->rows == OLED_ROW32) ? 2 : 1;

    while (n-- && *str) {
        if (oled->cursor_x >= OLED_COL) {
            CHECK_ERROR(set_oled_cursor(oled, 0, (oled->cursor_y + dy)&0x7));
            return SUCCESS;
        }

        if (*str == '\n') {
            CHECK_ERROR(set_oled_cursor(oled, 0, (oled->cursor_y + dy)&0x7));
        } else {
            toggle_oled(oled);
            CHECK_ERROR(print_char_oled(oled, *str));
            toggle_oled(oled);
        }
        str++;
    }

    return SUCCESS;
}

uint32_t toggle_oled(struct oled *oled) {

    uint32_t cmd;
    if (oled->on) {
        cmd = 0xAE;
        oled->on = 0;
    } else {
        cmd = 0xAF;
        oled->on = 1;
    }
    CHECK_ERROR(send_cmd_oled(oled, &cmd, 1));
    return SUCCESS;
}

uint32_t print_oled(struct oled *oled, char *str) {
    CHECK_ERROR(tx_oled_nstr(oled, str, OLED_COL * oled->rows));
    return SUCCESS;
}

uint32_t print_image_oled(struct oled *oled, const uint32_t *image) {

    clear_oled(oled);

    toggle_oled(oled);
    uint32_t start_data = 0x40;
    for (uint32_t y = 0; y < OLED_PAGE; y++) {
        start_i2c_tx(oled->i2c, oled->addr);
        CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, &start_data, 1), oled->i2c);
        for (uint32_t x = 0; x < OLED_COL; x++) {
            CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, image++, 1), oled->i2c);
        }
        end_i2c_tx(oled->i2c);

        CHECK_ERROR(set_oled_cursor(oled, 0, y+1));
    }
    toggle_oled(oled);

    return SUCCESS;
}

uint32_t config_scroll_oled(struct oled *oled, uint32_t start_page, uint32_t end_page) {

    uint32_t src[7];
    src[0] = 0x26; // Continuous horizontal scroll setup
    src[1] = 0x00;
    src[2] = start_page&0xFF; // Page0
    src[3] = 0x00; // 5 frames
    src[4] = end_page&0xFF; // Page1
    src[5] = 0x00;
    src[6] = 0xFF;
    CHECK_ERROR(send_cmd_oled(oled, src, 7));

    return SUCCESS;
}

uint32_t set_scroll_oled(struct oled *oled) {
    uint32_t src[2];
    src[0] = 0x00;
    src[1] = 0x2F; // Enable scrolling
    CHECK_NULLPTR(i2c_tx(oled->i2c, oled->addr, src, 2));

    return SUCCESS;
}

uint32_t unset_scroll_oled(struct oled *oled) {
    uint32_t src[2];
    src[0] = 0x00;
    src[1] = 0x2E; // Disable scrolling
    CHECK_NULLPTR(i2c_tx(oled->i2c, oled->addr, src, 2));

    return SUCCESS;
}
