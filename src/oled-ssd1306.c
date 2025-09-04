#include "periph/oled-ssd1306.h"
#include "types.h"
#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"
#include "font.h"
#include "oled_images/flower.h"

uint32_t oled_configured;

static uint32_t cursor_x;
static uint32_t cursor_y;

uint32_t send_cmd_oled(uint32_t addr, uint32_t *cmd, uint32_t n) {
    uint32_t start_cmd = 0x00;

    start_i2c_tx(addr);
    CHECK_NULLPTR_ENDTX(i2c_tx(NO_COND, &start_cmd, 1));
    CHECK_NULLPTR_ENDTX(i2c_tx(NO_COND, cmd, n));
    end_i2c_tx();

    return SUCCESS;
}

uint32_t send_data_oled(uint32_t addr, uint32_t *data, uint32_t n) {
    uint32_t start_data = 0x40;

    start_i2c_tx(addr);
    CHECK_NULLPTR_ENDTX(i2c_tx(NO_COND, &start_data, 1));
    CHECK_NULLPTR_ENDTX(i2c_tx(NO_COND, data, n));
    end_i2c_tx();

    return SUCCESS;
}

uint32_t set_oled_cursor(uint32_t addr, uint32_t x, uint32_t y) {
    uint32_t cmd;

    cmd = 0xB0 | (y&0x7);
    CHECK_ERROR(send_cmd_oled(addr, &cmd, 1));

    cmd = 0x00 | (x&0xF);
    CHECK_ERROR(send_cmd_oled(addr, &cmd, 1));

    cmd = 0x10 | ((x >> 4)&0xF);
    CHECK_ERROR(send_cmd_oled(addr, &cmd, 1));

    return SUCCESS;
}

#define TRIES 150
uint32_t config_oled(uint32_t addr, uint32_t rows) {
    vint32_t tries = TRIES;
    while (tries-- && start_i2c_tx(addr));

    uint32_t src[8];

    if (tries < 0) {
        return FAILURE;
    } else {
        end_i2c_tx();
        delay(4);

        if (rows == OLED_PAGE32) {
            src[0] = OLED_DATA(0xA8);
            src[1] = OLED_DATA(0x1F); // 31
            send_cmd_oled(addr, src, 2);
        }

        // Charge pump ON (MUST)
        src[0] = OLED_DATA(0x8D);
        src[1] = OLED_DATA(0x14);
        send_cmd_oled(addr, src, 2);

        // Display ON
        src[0] = OLED_DATA(0xAF);
        send_cmd_oled(addr, src, 2);

        src[0] = 0x00;
        src[1] = 0x26; // Continuous horizontal scroll setup
        src[2] = 0x00;
        src[3] = 0x00; // Page0
        src[4] = 0x00; // 5 frames
        src[5] = 0x01; // Page1
        src[6] = 0x00;
        src[7] = 0xFF;
        send_cmd_oled(addr, src, 8);

        clear_oled(addr);

        oled_configured = 1;
        return SUCCESS;
    }
}

uint32_t clear_oled(uint32_t addr) {
    uint32_t data = 0;

    uint32_t start_data = 0x40;
    for (uint32_t y = 0; y < OLED_PAGE64; y++) {
        start_i2c_tx(addr);
        CHECK_NULLPTR_ENDTX(i2c_tx(NO_COND, &start_data, 1));
        for (uint32_t x = 0; x < OLED_COL; x++) {
            CHECK_NULLPTR_ENDTX(i2c_tx(NO_COND, &data, 1));
        }
        end_i2c_tx();

        CHECK_ERROR(set_oled_cursor(addr, 0, y+1));
    }
    CHECK_ERROR(set_oled_cursor(addr, 0, 0));

    return SUCCESS;
}

uint32_t print_char_oled(uint32_t addr, uint32_t c) {
    for (uint32_t col = 0; col < 6; col++) {
        uint32_t data = font5x7[c - 0x20][col];
        CHECK_ERROR(send_data_oled(addr, &data, 1));
    }
    return SUCCESS;
}

// Similar to tx_lcd_nstr, might combine both and add general function to i2c.c
static uint32_t tx_oled_nstr(uint32_t addr, char *str, uint32_t n) {
    uint32_t stop = 0;
    while (n-- && *str) {
        if (cursor_x >= OLED_COL)
            stop = 1;
        if (*str == '\n' || stop) {
            cursor_y = (cursor_y + 1) % OLED_PAGE64;
            cursor_x = 0;
            CHECK_ERROR(set_oled_cursor(addr, 0, cursor_y));
            if (stop) {
                return SUCCESS;
            }
        } else {
            CHECK_ERROR(print_char_oled(addr, *str));
            cursor_x++;
        }
        str++;
    }

    return SUCCESS;
}

uint32_t print_oled(uint32_t addr, uint32_t tot, char *str) {
    start_i2c_tx(addr);
    CHECK_ERROR_ENDTX(tx_oled_nstr(addr, str, tot));
    end_i2c_tx();

    return SUCCESS;
}

uint32_t print_image_oled(uint32_t addr, const uint32_t *image) {

    clear_oled(addr);

    uint32_t start_data = 0x40;
    for (uint32_t y = 0; y < OLED_PAGE64; y++) {
        start_i2c_tx(addr);
        CHECK_NULLPTR_ENDTX(i2c_tx(NO_COND, &start_data, 1));
        for (uint32_t x = 0; x < OLED_COL; x++) {
            CHECK_NULLPTR_ENDTX(i2c_tx(NO_COND, image++, 1));
        }
        end_i2c_tx();

        CHECK_ERROR(set_oled_cursor(addr, 0, y+1));
    }
    CHECK_ERROR(set_oled_cursor(addr, 0, 0));

    return SUCCESS;
}

uint32_t config_scroll_oled(uint32_t addr, uint32_t start_page, uint32_t end_page) {

    uint32_t src[8];
    src[0] = 0x00;
    src[1] = 0x26; // Continuous horizontal scroll setup
    src[2] = 0x00;
    src[3] = start_page&0xFF; // Page0
    src[4] = 0x00; // 5 frames
    src[5] = end_page&0xFF; // Page1
    src[6] = 0x00;
    src[7] = 0xFF;
    CHECK_ERROR(send_cmd_oled(addr, src, 8));

    return SUCCESS;
}

uint32_t set_scroll_oled(uint32_t addr) {
    uint32_t src[2];
    src[0] = 0x00;
    src[1] = 0x2F; // Enable scrolling
    CHECK_NULLPTR(i2c_tx(addr, src, 2));

    return SUCCESS;
}

uint32_t unset_scroll_oled(uint32_t addr) {
    uint32_t src[2];
    src[0] = 0x00;
    src[1] = 0x2E; // Disable scrolling
    CHECK_NULLPTR(i2c_tx(addr, src, 2));

    return SUCCESS;
}
