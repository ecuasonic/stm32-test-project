ENTRY(_reset);

MEMORY {
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 32K
    SRAM(xrw) : ORIGIN = 0x20000000, LENGTH = 10K
}
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

_Min_Heap_Size = 0x200;
_Min_Stack_Size = 0x400;

SECTIONS {

    .vectors : { 
        . = ALIGN(4);
        KEEP(*(.vectors)) 
        . = ALIGN(4);
    } > FLASH

    .text : { 
        . = ALIGN(4);
        *(.text*) 

        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    .rodata : { 
        . = ALIGN(4);
        *(.rodata) 
        *(.rodata*) 
        . = ALIGN(4);
    } > FLASH

    .data : {
        . = ALIGN(4);
        _sdata = .;             /* data section start */
        *(.data)
        *(SORT(.data.*))
        *(.RamFunc)
        *(.RamFunc*)
        . = ALIGN(4);
        _edata = .;             /* data section end */
    } > SRAM AT > FLASH
    _sidata = LOADADDR(.data);

    . = ALIGN(4);
    .bss : {
        _sbss = .;              /* bss section start */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;              /* bss section end */
        __bss_end__ = _ebss;
    } > SRAM

    ._user_heap_stack : {
        . = ALIGN(8);
        PROVIDE(end = .);
        PROVIDE(_end = .);
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } > SRAM

}
. = ALIGN(8);
_end = .;          /* for cmsis_gcc.h */
