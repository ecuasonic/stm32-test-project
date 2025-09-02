#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

#include "types.h"

#define OLED_I2C_ADDR1 0x3C
#define OLED_I2C_ADDR2 0x3D

extern uint32_t oled_configured;

uint32_t config_oled(void);

#endif
