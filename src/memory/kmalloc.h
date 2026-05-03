#pragma once
#include <stdint.h>
#include "../serial/serial.h"
#include "pmm.h"
#include "vmm.h"
#include "buddy.h"
#include "slab.h"

#define HEAP_START 0xFFFFFFFFE0000000
#define HEAP_SIZE  0x1000000

void init_kheap(void);
void *kmalloc(size_t size);
void kfree(void *ptr);