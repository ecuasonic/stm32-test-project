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

#define CHECK_ERROR(n)      if (n) return FAILURE
#define CHECK_NULLPTR(n)    if (!n) return FAILURE

#endif
