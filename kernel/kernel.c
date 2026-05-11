#include <stdint.h>
#include <multiboot2.h>
#include <drivers/serial.h>
#include <drivers/lfb.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <drivers/pit.h>
#include <arch/x86_64/inlineasm.h>
#include <mm/kmalloc.h>
#include <drivers/acpi.h>
#include <arch/x86_64/apic/ioapic.h>

void kernel_main(uint64_t magic, unsigned int physBootInfo){
    serial_init();
    init_GDT();
    init_IDT();
    init_PIT(10);

    init_PMM(physBootInfo);
    init_VMM(physBootInfo);
    
    init_kheap();

    init_LFB(physBootInfo);

    parse_acpi(physBootInfo);
    init_LAPIC();
    init_IOAPIC();

    sti();

    for (;;);
}
