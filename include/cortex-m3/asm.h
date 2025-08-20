#ifndef ASM_H
#define ASM_H

#define INLINE static inline

INLINE void wfe(void) {
    __asm__ volatile ("wfe" : : :);
}
INLINE void wfi(void) {
    __asm__ volatile ("wfi" : : :);
}
INLINE void sev(void) {
    __asm__ volatile ("sev" : : :);
}

INLINE void cpsid(void) {
    __asm__ volatile ("cpsid i" : : :);
}
INLINE void cpsie(void) {
    __asm__ volatile ("cpsie i" : : :);
}

#endif // ASM_H