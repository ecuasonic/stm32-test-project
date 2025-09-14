#include "periph/oled-ssd1306.h"
#include "types.h"
#include "core_stm/i2c.h"
#include "font.h"
#include "cortex-m3/nvic/systick.h"

uint32_t pa = 1; // Page Addressing / Horizontal Addressing

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
        delay_ms(1);
        oled->addr = addr;
        oled->rows = rows;
        oled->cursor_x = 0;
        oled->cursor_y = 0;
        oled->i2c = i2c;
        oled->pa_mem = 1;

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
        CHECK_ERROR(oled_cmd_toggle_display(oled));
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

uint32_t set_oled_cursor_pa(struct oled *oled, uint32_t x, uint32_t y) {
    if (x >= OLED_COL) {
        return FAILURE;
    }

    if (oled->pa_mem) {
        oled_cmd_set_col_pa(oled, x);
        oled_cmd_set_page_start_pa(oled, y);

        oled->cursor_x = x&0xFF;
        oled->cursor_y = y&0x7;

        return SUCCESS;
    }
    return FAILURE;
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
            CHECK_ERROR(set_oled_cursor_pa(oled, oled->cursor_x, oled->cursor_y+1));

            // Higher nibble (0x X D7 X D6 X D5 X D4) on page n+1
            data4 = (data >> 4)&0xF;
            proc = 0;
            for (uint32_t i = 0; i < 4; i++) {
                proc |= ((data4 >> i)&0x1) << i*2;
            }
            CHECK_ERROR(send_data_oled(oled, &proc, 1));
            CHECK_ERROR(set_oled_cursor_pa(oled, oled->cursor_x+1, oled->cursor_y-1));
        } else if (oled->rows == 64){
            CHECK_ERROR(send_data_oled(oled, &data, 1));
            CHECK_ERROR(set_oled_cursor_pa(oled, oled->cursor_x+1, oled->cursor_y));
        }
    }
    return SUCCESS;
}

// Similar to tx_lcd_nstr, might combine both and add general function to i2c.c
static uint32_t tx_oled_nstr(struct oled *oled,char *str, uint32_t n) {
    uint32_t dy = (oled->rows == OLED_ROW32) ? 2 : 1;

    while (n-- && *str) {

        if (*str == '\n') {
            CHECK_ERROR(set_oled_cursor_pa(oled, 0, (oled->cursor_y + dy)&0x7));
        } else if (oled->cursor_x >= OLED_COL-6) {
            CHECK_ERROR(set_oled_cursor_pa(oled, 0, (oled->cursor_y + dy)&0x7));
            // return SUCCESS;
        } else {
            // CHECK_ERROR(oled_cmd_toggle_display(oled));
            CHECK_ERROR(print_char_oled(oled, *str));
            // CHECK_ERROR(oled_cmd_toggle_display(oled));
        }
        str++;
    }

    return SUCCESS;
}

uint32_t print_oled(struct oled *oled, char *str) {
    CHECK_ERROR(tx_oled_nstr(oled, str, OLED_COL * oled->rows));
    return SUCCESS;
}

uint32_t clear_oled(struct oled *oled) {
    uint32_t start_data = 0x40;
    uint32_t data = 0;

    CHECK_ERROR(oled_cmd_toggle_display(oled));
    CHECK_ERROR(oled_cmd_toggle_mem(oled));
    CHECK_ERROR(oled_cmd_set_col(oled, 0, OLED_COL-1));
    CHECK_ERROR(oled_cmd_set_page(oled, 0, OLED_PAGE-1));

    start_i2c_tx(oled->i2c, oled->addr);
    CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, &start_data, 1), oled->i2c);
    for (uint32_t i = 0; i < OLED_PAGE * OLED_COL; i++) {
        CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, &data, 1), oled->i2c);
    }
    end_i2c_tx(oled->i2c);

    CHECK_ERROR(oled_cmd_toggle_display(oled));
    CHECK_ERROR(oled_cmd_toggle_mem(oled));
    CHECK_ERROR(set_oled_cursor_pa(oled, 0, 0));

    return SUCCESS;
}


uint32_t print_image_oled(struct oled *oled, const uint32_t *image) {
    CHECK_ERROR(oled_cmd_toggle_display(oled));
    CHECK_ERROR(oled_cmd_toggle_mem(oled));
    CHECK_ERROR(oled_cmd_set_col(oled, 0, OLED_COL-1));
    CHECK_ERROR(oled_cmd_set_page(oled, 0, OLED_PAGE-1));

    CHECK_ERROR(send_data_oled(oled, (uint32_t *)image, OLED_COL * OLED_PAGE));

    CHECK_ERROR(oled_cmd_toggle_display(oled));
    CHECK_ERROR(oled_cmd_toggle_mem(oled));

    return SUCCESS;
}

uint32_t print_image_oled_dma(struct oled *oled, const uint32_t *image) {
    CHECK_ERROR(oled_cmd_toggle_display(oled));
    CHECK_ERROR(oled_cmd_toggle_mem(oled));
    CHECK_ERROR(oled_cmd_set_col(oled, 0, OLED_COL-1));
    CHECK_ERROR(oled_cmd_set_page(oled, 0, OLED_PAGE-1));

    // Start DMA transfer
    uint32_t start_data = 0x40;
    start_i2c_tx(oled->i2c, oled->addr);
    CHECK_NULLPTR_ENDTX(i2c_tx(oled->i2c, NO_COND, &start_data, 1), oled->i2c);
    i2c_tx_dma(oled->i2c, image, 1024); // DMA intr ends i2c transaction

    // This makes DMA useless, but I just want to learn how to enable DMA
    vuint32_t *cr2 = &oled->i2c->CR2;
    while (*cr2 & I2C_CR2_DMAEN);
    CHECK_ERROR(oled_cmd_toggle_display(oled));
    CHECK_ERROR(oled_cmd_toggle_mem(oled));

    return SUCCESS;
}
