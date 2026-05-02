#pragma once
#include <stdint.h>
#include "../serial/serial.h"
#include "pmm.h"
#include "vmm.h"
#include "buddy.h"

#define HEAP_START 0xFFFFFFFFE0000000
#define HEAP_SIZE  0x1000000

void init_kheap(void);