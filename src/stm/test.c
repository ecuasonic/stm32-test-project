#include "test.h"

void image_demo(void) {
    ASSERT(print_oled(&oled32, "Printing on OLED 32"));
    ASSERT(print_oled(&oled64, "Printing on OLED 64"));
    ASSERT(print_lcd(&lcd, "Printing on LCD"));

    sleep_ms(1000);

    ASSERT(clear_oled(&oled32));
    ASSERT(print_oled(&oled32, "DMA works\nOLED is done"));
    ASSERT(clear_oled(&oled64));
    ASSERT(print_oled(&oled64, "Very nice\nThis finally works\nOLED is done"));

    sleep_ms(1000);

    ASSERT(print_image_oled_dma(&oled64, flower));

    sleep_ms(1000);

    // FIX: After a long time, a second image looks unrecognizable.
    ASSERT(print_image_oled_dma(&oled64, flower2));

    ASSERT(oled_cmd_setup_hort_scroll(&oled64));
    ASSERT(oled_cmd_toggle_scroll(&oled64));

    for (uint32_t i = 0; i < 5; i++) {
        delay_ms(1000);
        ASSERT(oled_cmd_toggle_scroll(&oled64));
        delay_ms(1000);
        ASSERT(oled_cmd_setup_hort_scroll(&oled64));
        ASSERT(oled_cmd_toggle_scroll(&oled64));
    }
}

void acc_demo(void) {
    print_acc_data_lcd(&lcd, &acc, 'A', 5, 100);
    print_acc_test_lcd(&lcd, &acc, 'A', 5);
}
