#pragma once
#include <stdint.h>
#include <stddef.h>
#include <mm/kmalloc.h>
#include <drivers/lfb.h>

#define DEFAULT_STACK_SIZE 0x4000

void *alloc_stack(size_t size);