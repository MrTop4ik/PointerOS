#pragma once
#include <stdint.h>
#include "../multiboot2.h"
#include "../utils/string.h"
#include "../serial/serial.h"

void init_PMM(unsigned int bootInfoAddr);
uint64_t pmm_alloc_page(void);