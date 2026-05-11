#pragma once
#include <stdint.h>
#include <stddef.h>
#include <drivers/acpi.h>
#include <mm/kmalloc.h>

#define LAPIC_VIRT  0xFFFFFFFFFC000000

#define LAPIC_EOI_OFFSET 0xB0
#define LAPIC_SVR_OFFSET 0xF0

#define IA32_APIC_MSR 0x1B

#define LAPIC_SVR_ENABLE (1ULL << 8)
#define IA32_APIC_ENABLE (1ULL << 11)

void lapic_eoi(void);