#pragma once
#include <stdint.h>
#include "../utils/utils.h"

struct idt_entry_struct {
    uint16_t base_low;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
}__attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

void setPIC(void);
void setIDTGate(uint8_t num, uint32_t base);
void init_IDT(void);
void isr_handler(struct InterruptRegisters *regs);
void setIRQHandler(uint8_t num, void (*handler)(struct InterruptRegisters *regs));
void deleteIRQHandler(uint8_t num);
void irq_handler(struct InterruptRegisters *regs);