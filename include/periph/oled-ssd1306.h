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
#define OLED_PAGE32  4
#define OLED_PAGE64  8
#define OLED_TOT64   (OLED_PAGE64 * OLED_COL)
#define OLED_TOT32   (OLED_PAGE32 * OLED_COL)

extern uint32_t oled_configured;

uint32_t config_oled(uint32_t addr, uint32_t rows);
uint32_t send_cmd_oled(uint32_t addr, uint32_t *cmd, uint32_t n);
uint32_t send_data_oled(uint32_t addr, uint32_t *data, uint32_t n);
uint32_t set_oled_cursor(uint32_t addr, uint32_t x, uint32_t y);
uint32_t clear_oled(uint32_t addr);
uint32_t print_char_oled(uint32_t addr, uint32_t c);
uint32_t print_oled(uint32_t addr, uint32_t rows, char *str);

#endif
