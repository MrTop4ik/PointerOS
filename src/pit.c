#include "pit.h"

volatile uint64_t ticks;

void init_PIT(uint32_t freq){
    ticks = 0;
    uint32_t division = 1193180 / freq;

    setIRQHandler(0, &PIT_handler);
    
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(division & 0xFF));
    outb(0x40, (uint8_t)((division >> 8) & 0xFF));
}

void PIT_handler(struct InterruptRegisters *regs){
    ticks++;
}