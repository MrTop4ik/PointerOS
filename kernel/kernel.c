#include <stdint.h>
#include <multiboot2.h>
#include <drivers/serial.h>
#include <drivers/lfb.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <arch/x86_64/pit.h>
#include <arch/x86_64/inlineasm.h>
#include <kernel/mm/kmalloc.h>
#include <arch/x86_64/apic.h>

void kernel_main(uint64_t magic, unsigned int physBootInfo){
    serial_init();
    init_GDT();
    init_IDT();
    init_PIT(10);

    init_PMM(physBootInfo);
    init_VMM(physBootInfo);
    
    init_kheap();

    init_LFB(physBootInfo);

    init_APIC(physBootInfo);

    sti();

    for (;;);
}
