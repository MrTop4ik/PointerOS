#pragma once
#include <stdint.h>
#include <drivers/serial.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/buddy.h>
#include <mm/slab.h>

#define HEAP_START 0xFFFFFFFFE0000000
#define HEAP_SIZE  0x1000000

void init_kheap(void);
void *kmalloc(size_t size);
void kfree(void *ptr);