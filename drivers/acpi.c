#include <drivers/acpi.h>

uint64_t lapic_paddr, ioapic_paddr;
uint32_t iso_count = 0;
struct MADT_ISO iso_list[MAX_ISO];

void parse_acpi(unsigned int physBootInfo){
    struct multiboot_info* virtBootInfo = (struct multiboot_info *)(physBootInfo + DIRECT_OFFSET);

    struct multiboot_tag* tag = (struct multiboot_tag *)((uint8_t *)virtBootInfo + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END){
        if (tag->type == MULTIBOOT_TAG_TYPE_ACPI_NEW){
            struct multiboot_tag_new_acpi *acpi = (struct multiboot_tag_new_acpi *)tag;
            struct RSDPDescriptor20 *rsdp = (struct RSDPDescriptor20 *)(acpi->rsdp);
            struct MADT *madt = find_madt(rsdp->XsdtAddress);
            parse_madt(madt);
            return;
        }
        tag = (struct multiboot_tag *)((uintptr_t)((uint8_t*)tag + tag->size + 7) & ~7);
    }
}

struct MADT *find_madt(uint64_t xsdt_phys){
    struct ACPISDTHeader *xsdt = (struct ACPISDTHeader *)(xsdt_phys + DIRECT_OFFSET);

    int entries = (xsdt->Length - sizeof(struct ACPISDTHeader)) / 8;
    uint64_t *ptrs = (uint64_t *)((uint8_t *)xsdt + sizeof(struct ACPISDTHeader));

    for (int i = 0; i < entries; i++){
        struct ACPISDTHeader *h = (struct ACPISDTHeader *)(ptrs[i] + DIRECT_OFFSET);
        if (memcmp(h->Signature, "APIC", 4) == 0){
            return (struct MADT *)h;
        }
    }
}

void parse_madt(struct MADT *madt){
    uint8_t *ptr = (uint8_t *)madt + sizeof(struct MADT);
    uint8_t *end = (uint8_t *)madt + madt->Header.Length;

    lapic_paddr = madt->LocalAPICAddress;
    
    while (ptr < end){
        struct MADTEntryHeader *entry = (struct MADTEntryHeader *)ptr;

        switch (entry->Type){
            case 0:
                break;
            case 1:
                struct MADT_IOAPIC *ioapic = (struct MADT_IOAPIC *)entry;
                ioapic_paddr = ioapic->IOApicAddress;
                break;
            case 2:
                struct MADT_ISO *iso = (struct MADT_ISO *)entry;
                save_iso(iso);
                break;
        }

        ptr += entry->Length;
    }
}

void save_iso(struct MADT_ISO *iso){
    if (iso_count < MAX_ISO) iso_list[iso_count++] = *iso;
    else kprintf("To many ISO.\n");
}