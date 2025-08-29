#include "types.h"
#include "periph/lcd.h"
#include "strings.h"

void reverse(char str[], uint32_t length) {
    uint32_t start = 0;
    uint32_t end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char *itoa(int32_t num, uint32_t base) {
    static char str[LCD_MAX_CHARS];

    uint32_t i = 0;
    uint32_t neg = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        neg = 1;
        num = -num;
    }

    while (num) {
        char rem = (char)((uint32_t)num % base);
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num /= base;
    }

    if (neg)
        str[i++] = '-';

    str[i] = '\0';
    reverse(str, i);
    return str;
}

