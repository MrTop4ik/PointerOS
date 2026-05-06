#pragma once
#include <stdint.h>
#include <arch/x86_64/idt.h>
#include <arch/x86_64/inlineasm.h>
#include <drivers/serial.h>

void init_PIT(uint32_t freq);
void PIT_handler(struct InterruptRegisters *regs);