#pragma once
#include <stdint.h>
#include "../multiboot2.h"
#include "../memory/vmm.h"
#include "../utils/inlineasm.h"

#define LFB_ADDR 0xFFFFFFFF8C000000

void init_LFB(unsigned int physBootInfo);