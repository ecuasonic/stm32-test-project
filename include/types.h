#ifndef TYPES_H
#define TYPES_H

#define INLINE static inline
#define SUCCESS 0
#define FAILURE 1

typedef unsigned int        uint32_t;
typedef volatile uint32_t   vuint32_t;
typedef signed int          int32_t;
typedef volatile int32_t    vint32_t;
typedef unsigned short      uint16_t;
typedef short               int16_t;
typedef volatile uint16_t   vuint16_t;
typedef unsigned char       uint8_t;

void gpio_clear(uint32_t bank, uint32_t pin); // Make sure this lines up with one in gpio.h
__attribute__((noreturn)) INLINE void panic(void) {
    gpio_clear('B', 10);
    for(;;);
}

#define CHECK_ERROR(n)      if (n) return FAILURE
#define CHECK_NULLPTR(n)    if (!n) return FAILURE
#define ASSERT(n)           if (n) panic()
#define ASSERT_NULLPTR(n)   if (!n) panic()

#endif
