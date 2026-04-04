#pragma once
#include <stdint.h>
#include "../utils/utils.h"

#define COM1_PORT 0x3F8

void serial_init(void);
void serial_putchar(char a);
void serial_print(const char* str);