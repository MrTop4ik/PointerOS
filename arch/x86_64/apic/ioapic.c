#include <arch/x86_64/apic/ioapic.h>

void init_IOAPIC(void){
    vmm_map_page(read_cr3(), ioapic_paddr, IOAPIC_VIRT, 0x1000, PTE_WRITABLE | PTE_CD);
}

uint32_t get_gsi_for_irq(uint8_t irq){
    for (int i = 0; i < iso_count; i++){
        if (iso_list[i].IRQSource == irq){
            return iso_list[i].GSI;
        }
    }
    return (uint32_t)irq;
}