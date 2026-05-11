#include <arch/x86_64/apic/lapic.h>

void lapic_eoi(void){
    *(volatile uint32_t *)(LAPIC_VIRT + LAPIC_EOI_OFFSET) = 0;
}