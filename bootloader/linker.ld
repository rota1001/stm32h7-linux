MEMORY {
    DTCMRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    SRAM1 (rwx) : ORIGIN = 0x30000000, LENGTH = 288K
    SRAM4 (rwx) : ORIGIN = 0x38000000, LENGTH = 64K
    FLASH (rx) : ORIGIN = 0x8000000, LENGTH = 128K
    AXISRAM (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
}

SECTIONS {
    . = 0x8000000;
    .isr_vector : {
        KEEP(*(.isr_vector))
        KEEP(*(.reset_isr))
    } > FLASH
    . = ALIGN(4);
    .text : {
        *(.text)
        *(.rodata)
    } > FLASH
    .data : {
        . = ORIGIN(SRAM4) + 10K;
        _data_vma_start = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _data_vma_end = .;
    } > SRAM4 AT > FLASH
    .bss (NOLOAD) : {
        . = ALIGN(4);
        _bss_vma_start = .;
        *(.bss)
        . = ALIGN(4);
        _bss_vma_end = .;
    } > SRAM4
    _stack_top = ORIGIN(SRAM4) + 10K;
    _data_lma = LOADADDR(.data);
}