#pragma once
#include <stdint.h>
#include "vga/vga.h"
#include "utils/utils.h"
#include "idt/idt.h"

void irq1_handler(struct InterruptRegisters *regs);
void init_Keyboard(void);