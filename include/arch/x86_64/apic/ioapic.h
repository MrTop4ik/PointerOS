#pragma once
#include <stdint.h>
#include <stddef.h>
#include <drivers/acpi.h>
#include <mm/kmalloc.h>
#include <arch/x86_64/inlineasm.h>

#define IOAPIC_VIRT 0xFFFFFFFFFC100000

void init_IOAPIC(void);
uint32_t get_gsi_for_irq(uint8_t irq);