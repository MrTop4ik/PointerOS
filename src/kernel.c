#include "vga/vga.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "pit.h"
#include "utils/utils.h"
#include "keyboard.h"
#include "memory.h"
#include "multiboot.h"

void kernel_main(uint32_t magic, struct multiboot_info *bootInfo){
    term_init();
    init_GDT();
    init_IDT();
    init_PIT(10);
    init_Keyboard();

    uint32_t mod1 = *(uint32_t*)(bootInfo->mods_addr + 4);
    uint32_t psysicalAllocStart = (mod1 + 0xFFF) & ~0XFFF;

    init_Memory(bootInfo->mem_upper * 1024, psysicalAllocStart);

    for (;;);
}