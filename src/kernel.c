#include <stdint.h>
#include "multiboot2.h"
#include "utils/utils.h"
#include "serial/serial.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "pit.h"

void kernel_main(uint64_t magic, struct multiboot_info* bootInfo){
    serial_init();
    init_GDT();
    init_IDT();
    init_PIT(10);

    for (;;);
}