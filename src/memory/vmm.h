#pragma once
#include <stdint.h>
#include "../multiboot2.h"
#include "../utils/string.h"
#include "../serial/serial.h"

void init_VMM(unsigned int bootInfoAddr);