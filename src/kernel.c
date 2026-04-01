#include "vga/vga.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "pit.h"
#include "utils/utils.h"
#include "keyboard.h"
#include "multiboot.h"
#include "memory.h"

void kernel_main(uint32_t magic, unsigned int physBootInfoAddr){
    term_init();
    init_GDT();
    init_IDT();
    init_PIT(10);
    init_Keyboard();

    struct multiboot_info *bootInfo = (struct multiboot_info *)((physBootInfoAddr) + KERNEL_START);

    uint32_t physicalAllocStart = 0x1100000;

    if (bootInfo->flags & (1 << 3) && bootInfo->mods_count > 0){
        uint32_t mods_virt_addr = (uint32_t)bootInfo->mods_addr + KERNEL_START;

        uint32_t last_mod_index = bootInfo->mods_count - 1;
        uint32_t last_mod_ptr = mods_virt_addr + (last_mod_index * 16);

        uint32_t last_mod_end_phys = *(uint32_t*)(last_mod_ptr + 4);

        if (last_mod_end_phys > physicalAllocStart){
            physicalAllocStart = (last_mod_end_phys + 0xFFF) & ~0xFFF;
        }
    }

    init_Memory(bootInfo->mem_upper * 1024, physicalAllocStart, bootInfo);
    
    for (;;);
}