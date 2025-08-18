#ifndef ASM_H
#define ASM_H

#define INLINE static inline

INLINE void wfe(void) {
    __asm__ ("wfe");
}
INLINE void wfi(void) {
    __asm__ ("wfi");
}

#endif // ASM_H