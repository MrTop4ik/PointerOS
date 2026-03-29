#pragma once
#include <stdint.h>
#include "utils/utils.h"
#include "idt/idt.h"
#include "vga/vga.h"

void irq0_handler(struct InterruptRegisters *regs);
void init_PIT(uint32_t freq);