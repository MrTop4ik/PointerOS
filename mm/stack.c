#include <mm/stack.h>

void *alloc_stack(size_t size){
    if (size == 0) size = DEFAULT_STACK_SIZE;

    void *stack_bottom = kmalloc(size);
    if (!stack_bottom) return NULL;

    return (uint8_t*)stack_bottom + size;
}