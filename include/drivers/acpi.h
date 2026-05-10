#pragma once
#include <stdint.h>
#include <stddef.h>
#include <drivers/lfb.h>
#include <arch/x86_64/inlineasm.h>
#include <libc/string.h>
#include <mm/kmalloc.h>
#include <multiboot2.h>

#define LAPIC_VIRT  0xFFFFFFFFFC000000
#define IOAPIC_VIRT 0xFFFFFFFFFC100000

#define LAPIC_EOI_OFFSET 0xB0
#define LAPIC_SVR_OFFSET 0xF0

#define IA32_APIC_MSR 0x1B

#define LAPIC_SVR_ENABLE (1ULL << 8)
#define IA32_APIC_ENABLE (1ULL << 11)

struct RSDPDescriptor20 {
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t Reserved[3];
}__attribute__((packed));

struct ACPISDTHeader {
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableD[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
}__attribute__((packed));

struct MADT {
    struct ACPISDTHeader Header;
    uint32_t LocalAPICAddress;
    uint32_t Flags;
}__attribute__((packed));

struct MADTEntryHeader {
    uint8_t Type;
    uint8_t Length;
}__attribute__((packed));

struct MADT_LocalAPIC {
    struct MADTEntryHeader Header;
    uint8_t ProcessorID;
    uint8_t ApicID;
    uint32_t Flags;
}__attribute__((packed));

struct MADT_IOAPIC {
    struct MADTEntryHeader Header;
    uint8_t IOApicID;
    uint8_t Reserved;
    uint32_t IOApicAddress;
    uint32_t GlobalSystemInterruptBase;
}__attribute__((packed));

struct MADT_ISO {
    struct MADTEntryHeader Header;
    uint8_t BusSource;
    uint8_t IRQSource;
    uint32_t GSI;
    uint16_t Flags;
}__attribute__((packed));

extern uint64_t lapic_paddr;
extern uint64_t ioapic_paddr;

void parse_acpi(unsigned int physBootInfo);
struct MADT* find_madt(uint64_t xsdt_phys);
void parse_madt(struct MADT *madt);