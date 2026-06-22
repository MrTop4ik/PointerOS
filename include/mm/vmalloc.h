#pragma once 
#include <stdint.h>
#include <mm/kmalloc.h>

#define VMALLOC_START 0xFFFFC00000000000

typedef struct vm_info {
    void *base;
    size_t size;
    struct vm_info *next;
    struct vm_info *prev;
} vm_info_t;

extern vm_info_t *vmalloc_list_head;

void *vmalloc(size_t size);
void vfree(void *ptr);
void vm_add_to_list(vm_info_t *i);
void vm_remove_from_list(vm_info_t *i);