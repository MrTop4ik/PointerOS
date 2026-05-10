#pragma once
#include <stdint.h>
#include <drivers/serial.h>
#include <kernel/mm/pmm.h>
#include <kernel/mm/vmm.h>
#include <kernel/mm/buddy.h>
#include <kernel/mm/slab.h>

#define HEAP_START 0xFFFFFFFFE0000000
#define HEAP_SIZE  0x1000000

void init_kheap(void);
void *kmalloc(size_t size);
void kfree(void *ptr);