#include "gdt.h"
#include "../utils/utils.h"

struct gdt_entry_struct gdt_entries[6];
struct gdt_ptr_struct gdt_ptr;
struct tss_entry_struct tss_entry;

extern void gdt_flush(uint32_t );
extern void tss_flush();

void setGDTGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_low = (base >> 16) & 0xFF;
    gdt_entries[num].base_low = (base >> 24) & 0xFF;
    
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].flags = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    
    gdt_entries[num].access = access;
}

void setTSS(uint32_t num, uint16_t ss0, uint32_t esp0){
    uint32_t base = (uint32_t)&tss_entry;
    uint32_t limit = sizeof(tss_entry) - 1;

    setGDTGate(num, base, limit, 0x89, 0x00);
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;

    tss_entry.cs = 0x08 | 0x3;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10 | 0x3;
}

void init_GDT(){
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 6) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    setGDTGate(0,0,0,0,0);
    setGDTGate(1,0,0xFFFFF,0x9A,0xCF);
    setGDTGate(2,0,0xFFFFF,0x92,0xCF);
    setGDTGate(3,0,0xFFFFF,0xFA,0xCF);
    setGDTGate(4,0,0xFFFFF,0xF2,0xCF);
    setTSS(5, 0x10, 0x08);

    gdt_flush((uint32_t)&gdt_ptr);
    tss_flush();
}