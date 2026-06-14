#pragma once
#include <stdint.h>
#include <stddef.h>
#include <mm/kmalloc.h>
#include <drivers/lfb.h>

#define DEFAULT_STACK_SIZE 0x4000
#define USER_STACK_MAX 0x00007FFFFFFFFFFF

typedef struct {
    void *top;
    void *bottom;
    size_t size;
} stack_t;

void *kernel_alloc_stack(size_t size);
void *user_alloc_stack(size_t size);
void user_free_stack(void *stack_bottom, size_t size);