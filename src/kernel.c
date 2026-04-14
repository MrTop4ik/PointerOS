#include <stdint.h>
#include "multiboot2.h"
#include "serial/serial.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "pit.h"

void kernel_main(uint64_t magic, unsigned int physBootInfo){
    serial_init();
    init_GDT();
    init_IDT();
    init_PIT(10);

    struct multiboot_info* bootInfo = (struct multiboot_info *)(physBootInfo + 0xFFFFFFFF80000000);

    for (;;);
}