#include "periph/oled-ssd1306.h"
#include "types.h"
#include "core_stm/i2c.h"
#include "cortex-m3/nvic/systick.h"

#define TRIES 150

uint32_t oled_configured;

uint32_t config_oled(void) {
    vint32_t tries = TRIES;
    while (tries-- && start_i2c_tx(OLED_I2C_ADDR1));

    if (tries < 0) {
        return FAILURE;
    } else {
        delay(4);
        end_i2c_tx();
        oled_configured = 1;
        return SUCCESS;
    }
}
