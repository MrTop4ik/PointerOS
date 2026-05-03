#pragma once
#include <stdint.h>
#include "../serial/serial.h"
#include "pmm.h"
#include "vmm.h"
#include "kmalloc.h"

#define MAX_ORDERS 12
#define TOTAL_PAGES 4096

typedef struct free_block {
    struct free_block *prev;
    struct free_block *next;
} free_block_t;

typedef struct page_metadata {
    uint8_t order : 4;
    uint8_t is_free : 1;
    uint8_t is_slab: 1;
} page_metadata_t;

extern page_metadata_t metadata[TOTAL_PAGES];

void init_buddy(void);
void *buddy_alloc(int order);
void buddy_free(void *ptr);
int is_buddy_free(uint64_t buddy_addr, int order);
void buddy_list_remove(uint64_t addr, int order);
void buddy_list_add(uint64_t addr, int order);