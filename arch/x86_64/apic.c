#include <arch/x86_64/apic.h>

void init_APIC(unsigned int physBootInfo){
    struct multiboot_info* virtBootInfo = (struct multiboot_info *)(physBootInfo + DIRECT_OFFSET);

    struct multiboot_tag* tag = (struct multiboot_tag *)((uint8_t *)virtBootInfo + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END){
        if (tag->type == MULTIBOOT_TAG_TYPE_ACPI_NEW){
            struct RSDPDescriptor20 *rsdp = (struct RSDPDescriptor20 *)((uint8_t*)tag + 8);
            struct MADT *madt = find_madt(rsdp->XsdtAddress);
            serial_print("%llx\n", rsdp->XsdtAddress);
            kprintf("%llx\n", rsdp->XsdtAddress);
            return;
        }
        tag = (struct multiboot_tag *)((uintptr_t)((uint8_t*)tag + tag->size + 7) & ~7);
    }
}

struct MADT *find_madt(uint64_t xsdt){

}

void parse_madt(struct MADT *madt);