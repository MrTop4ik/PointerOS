ENTRY(_start)

PHYS_ADDR = 0x100000;
VIRT_ADDR = 0xFFFFFFFF80000000;

SECTIONS{
    kernel_start = PHYS_ADDR + VIRT_ADDR;

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
        *(.data)
    }

    .rodata : AT(ADDR(.rodata) - VIRT_ADDR) {
        *(.rodata)
    }

    .bss : AT(ADDR(.bss) - VIRT_ADDR) {
        *(COMMON)
        *(.bss)

        . = ALIGN(4096);
    }

    . += 0x20000;

    . = ALIGN(4096);

    kernel_end = .;
}