#ifndef TEST_H
#define TEST_H

#include "oled_images/flower.h"
#include "oled_images/flower2.h"
#include "types.h"
#include "cortex-m3/nvic/systick.h"
#include "periph/lcd-hd44780u.h"
#include "periph/oled-ssd1306.h"
#include "core_stm/usart.h"

extern struct oled oled32, oled64;
extern struct lcd lcd;
extern struct acc acc;

void image_demo(void);
void acc_demo(void);

#endif
