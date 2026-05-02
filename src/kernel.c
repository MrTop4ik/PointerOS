#include <stdint.h>
#include "multiboot2.h"
#include "serial/serial.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "pit.h"
#include "utils/inlineasm.h"
#include "memory/pmm.h"
#include "memory/vmm.h"
#include "memory/kmalloc.h"
#include "lfb/lfb.h"

void kernel_main(uint64_t magic, unsigned int physBootInfo){
    serial_init();
    init_GDT();
    init_IDT();
    init_PIT(10);

    init_PMM(physBootInfo);
    init_VMM(physBootInfo);
    
    init_kheap();

    init_LFB(physBootInfo);

    for (;;);
}
