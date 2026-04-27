#pragma once
#include <stdint.h>
#include "../multiboot2.h"
#include "../utils/string.h"
#include "../serial/serial.h"
#include "../utils/inlineasm.h"
#include "pmm.h"

void init_VMM(unsigned int bootInfoAddr);
void vmm_init_direct_mapping(uint64_t maxAddr);
void vmm_map_page(uint64_t pml4_phys, uint64_t paddr, uint64_t vaddr, uint64_t ps, uint64_t flags);