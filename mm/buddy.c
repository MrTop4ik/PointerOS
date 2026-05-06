#include <kernel/mm/buddy.h>

free_block_t *free_areas[MAX_ORDERS + 1];
page_metadata_t metadata[TOTAL_PAGES];

void init_buddy(void){
    for (int i = 0; i <= MAX_ORDERS; i++) free_areas[i] = NULL;
    
    for (int i = 0; i < TOTAL_PAGES; i++){
        metadata[i].is_free = 0;
        metadata[i].order = 0;
        metadata[i].is_slab = 0;
    }

    buddy_list_add(HEAP_START, MAX_ORDERS);
}

void *buddy_alloc(int order){
    for (int i = order; i <= MAX_ORDERS; i++){
        if (free_areas[i]){
            uint64_t block_addr = (uint64_t)free_areas[i];

            buddy_list_remove(block_addr, i);

            while (i > order){
                i--;
                uint64_t buddy_addr = block_addr ^ ((1ULL << i) * PAGE_SIZE_4KB);
                buddy_list_add(buddy_addr, i);
            }

            int index = (block_addr - HEAP_START) / PAGE_SIZE_4KB;
            metadata[index].order = order;
            metadata[index].is_free = 0;
            metadata[index].is_slab = 0;

            return (void *)block_addr;
        }
    }

    return NULL;
}

void buddy_free(void *ptr){
    uint64_t addr = (uint64_t)ptr;
    int index = (addr - HEAP_START) / PAGE_SIZE_4KB;
    
    int i = metadata[index].order;
    for(; i < MAX_ORDERS; i++){
        uint64_t buddy_addr = addr ^ ((1ULL << i) * PAGE_SIZE_4KB);
        int buddy_index = (buddy_addr - HEAP_START) / PAGE_SIZE_4KB;

        if (metadata[buddy_index].is_free && metadata[buddy_index].order == i){
            buddy_list_remove(buddy_addr, i);

            if (buddy_addr < addr){
                addr = buddy_addr;
            }
        } else {
            break;
        }
    }

    buddy_list_add(addr, i);
}

int is_buddy_free(uint64_t buddy_addr, int order){
    int index = (buddy_addr - HEAP_START) / PAGE_SIZE_4KB;
    return metadata[index].order == order && metadata[index].is_free;
}

void buddy_list_remove(uint64_t addr, int order){
    free_block_t *block = (free_block_t *)addr;
    if (block->prev) block->prev->next = block->next;
    else free_areas[order] = block->next;
    if (block->next) block->next->prev = block->prev;

    int index = (addr - HEAP_START) / PAGE_SIZE_4KB;
    metadata[index].is_free = 0;
}

void buddy_list_add(uint64_t addr, int order){
    free_block_t *block = (free_block_t *)addr;
    block->prev = NULL;
    block->next = free_areas[order];
    if (free_areas[order]) free_areas[order]->prev = block;

    free_areas[order] = block;

    int index = (addr - HEAP_START) / PAGE_SIZE_4KB;
    metadata[index].is_free = 1;
    metadata[index].is_slab = 0;
    metadata[index].order = order;
}