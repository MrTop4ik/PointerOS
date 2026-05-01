#pragma once
#include <stdint.h>
#include "../multiboot2.h"
#include "../memory/vmm.h"
#include "../utils/inlineasm.h"
#include "../serial/serial.h"
#include "font8x16.h"

#define LFB_ADDR 0xFFFFFFFFC0000000

void init_LFB(unsigned int physBootInfo);