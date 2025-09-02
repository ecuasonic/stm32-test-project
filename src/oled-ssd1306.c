#include "periph/oled-ssd1306.h"
#include "types.h"
#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"

#define TRIES 150

void config_oled(void) {
    vuint32_t tries = TRIES;
    while (tries-- && start_i2c_tx(OLED_I2C_ADDR1));
    end_i2c_tx();
    delay(4);
}
