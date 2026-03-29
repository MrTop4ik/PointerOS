#include "pit.h"

volatile uint32_t ticks;

void irq0_handler(struct InterruptRegisters *regs){
    ticks++;
}

void init_PIT(uint32_t freq){
    ticks = 0;
    uint32_t division = 1193180 / freq;

    setIRQHandler(0, &irq0_handler);
    
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(division & 0xFF));
    outb(0x40, (uint8_t)((division >> 8) & 0xFF));
}