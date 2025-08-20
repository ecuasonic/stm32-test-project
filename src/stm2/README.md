### Overview

## Things to look into

1. Backup registers
3. Sleep then wakeup (on interrupt or event)
    RTC to wakeup from low-power mode
        Similar to sleep().
2. Events
3. Power control
4. I2C (for screen peripherals)

## Success

1. Sleep using `wfe`.
2. Wakeup with EXTI1 (interrupt+event).

## Observations

I need to make the code easier to write, such as setup functions for gpio, interrupts, etc.

Apparently, it's not good to call `wfe`/`wfi` from interrupt subroutines. Call them in `main()` instead.

```c
// This is the recommended sleep handshake.
// wfe() will not go to sleep if event register is set, or if there are pending interrupts
void sleep() {
    cpsid();
    sev(); // set event register (no sleep)
    wfe(); // clear event register (no sleep)
    wfe(); // set event register (sleep)
    cpsie();
}
```

The `main()` code stops when sleeping. Resumes when it wakes back up.