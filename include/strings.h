#ifndef STRINGS_H
#define STRINGS_H

#include "types.h"
void reverse(char str[], uint32_t length);
char *itoa(int32_t num, uint32_t base, char *str, uint32_t n);
char *strndiff(char *s1, char *s2, uint32_t n);
uint32_t strnlen(char *s, uint32_t n);
uint32_t strncpy(char *s1, char *s2, uint32_t n);

#endif
