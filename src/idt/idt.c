#include <stdbool.h>
#include "idt.h"
#include "../vga/vga.h"

char *exceptions[] = {
    "Division By Zero.",
    "Debug.",
    "Non Maskable Interrupt.",
    "Breakpoint.",
    "Into Detected Overflow.",
    "Out Of Bounds.",
    "Invalid Opcode.",
    "No Coprocessor.",
    "Double Fault.",
    "Coprocessor Segment Overrun.",
    "Bad TSS.",
    "Segment not present.",
    "Stack fault.",
    "General Protection Fault.",
    "Page Fault.",
    "Unknown Interrupt.",
    "Coprocessor Fault.",
    "Aligment Fault.",
    "Machine Check.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
    "Reserved.",
};

void *irq_handlers[16] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct idt_ptr;

extern void idt_flush(uint32_t);
extern void *isrs[];
extern void isr48();
extern void isr128();
extern void isr177();
extern void *irqs[];

void setPIC(){
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

void setIDTGate(uint8_t num, uint32_t base){
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;

    idt_entries[num].always0 = 0;
    idt_entries[num].selector = 0x08;
    idt_entries[num].flags = 0x8E;
}

void init_IDT(){
    idt_ptr.limit = (sizeof(struct idt_entry_struct) * 256) - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    setPIC();

    for (uint16_t i = 0; i < 256; i++){
        setIDTGate(i, 0);
        idt_entries[i].flags = 0;
    }

    for (uint16_t j = 0; j  < 32; j++){
        setIDTGate(j, (uint32_t)isrs[j]);
    }

    for (uint16_t k = 0; k < 16; k++){
        setIDTGate(k + 32, (uint32_t)irqs[k]);
    }

    setIDTGate(48, (uint32_t)isr48);

    idt_flush((uint32_t)&idt_ptr);
}

void isr_handler(struct InterruptRegisters *regs){
    if (regs->int_no < 32){
        term_print(exceptions[regs->int_no]);
        term_print("\n");
        term_print("System Halted.\n");
        for (;;);
    }

    if (regs->int_no == 48){
        term_print("Int 0x30 Occured.\n");
        term_print("System Halted.\n");
        for (;;);
    }
}

void setIRQHandler(uint8_t num, void (*handler)(struct InterruptRegisters *regs)){
    irq_handlers[num] = handler;
}

void deleteIRQHandler(uint8_t num){
    irq_handlers[num] = 0;
}

void irq_handler(struct InterruptRegisters *regs){
    void (*handler)(struct InterruptRegisters *regs);

    handler = irq_handlers[regs->int_no - 32];

    handler(regs);

    if (regs->int_no >= 40){
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}