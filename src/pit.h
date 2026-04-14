#pragma once
#include <stdint.h>
#include "idt/idt.h"
#include "utils/inlineasm.h"
#include "serial/serial.h"

void init_PIT(uint32_t freq);
void PIT_handler(struct InterruptRegisters *regs);