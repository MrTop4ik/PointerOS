#pragma once
#include <stdint.h>
#include "../multiboot2.h"
#include "../utils/string.h"
#include "../serial/serial.h"
#include "../utils/inlineasm.h"
#include "pmm.h"

void init_VMM(unsigned int bootInfoAddr);
void vmm_init_direct_mapping(uint64_t maxAddr);