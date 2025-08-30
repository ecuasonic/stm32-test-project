#include "types.h"
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

char *itoa(int32_t num, uint32_t base, char *str, uint32_t n) {

    uint32_t i = 0;
    uint32_t neg = 0;

    if (n-- < 2) {
        return str;
    }

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        neg = 1;
        num = -num;
        n--;
    }

    while (num && n--) {
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

char *strndiff(char *s1, char *s2, uint32_t n) {
    while (n-- && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    if ( n == 0 )
        return 0;
    else
        return s2;
}

uint32_t strnlen(char *s, uint32_t n) {
    uint32_t len = 0;
    while (n-- && *s++) {
        len++;
    }
    return len;
}

uint32_t strncpy(char *s1, char *s2, uint32_t n) {
    uint32_t copied = 0;
    while (n-- && (*s1++ = *s2++)) {
        copied++;
    }
    s1[copied] = '\0';
    return copied;
}
