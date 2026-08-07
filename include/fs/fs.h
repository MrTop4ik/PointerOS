#pragma once
#include <stdint.h>
#include <multiboot2.h>
#include <libc/string.h>
#include <arch/x86_64/drivers/video/serial.h>
#include <arch/x86_64/inlineasm.h>
#include <mm/vmalloc.h>

typedef struct {
    uint32_t type;
    uint32_t size;
    uint64_t start;
    uint64_t end;
} ramdisk_t;

extern ramdisk_t ramdisk;

void parse_ramdisk(unsigned int bootInfoAddr);