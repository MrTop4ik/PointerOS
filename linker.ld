ENTRY(_start)

PHYS_ADDR = 0x100000;
VIRT_ADDR = 0xFFFFFFFF80000000;

SECTIONS{
    . = PHYS_ADDR;

    .boot : {
        KEEP(*(.multiboot_header))
        *(.boot_text)
        *(.boot_data)
        *(.boot_rodata)
        *(.boot_bss)
    }

    . += VIRT_ADDR;

    .text : AT(ADDR(.text) - VIRT_ADDR) {
        *(.text)
    }

    .data : AT(ADDR(.data) - VIRT_ADDR) {
        *(.rodata)
    }

    .rodata : AT(ADDR(.rodata) - VIRT_ADDR) {
        *(.data)
    }

    .bss : AT(ADDR(.bss) - VIRT_ADDR) {
        *(COMMON)
        *(.bss)
    }

    _kernel_end = .;
}