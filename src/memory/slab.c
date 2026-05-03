#include "slab.h"

kmem_cache_t kernel_caches[NUM_CACHES];

void init_slab(void){
    int sizes[] = {16, 32, 64, 128, 256, 512, 1024, 2048};
    for (int i = 0; i < NUM_CACHES; i++){
        kernel_caches[i].obj_size = sizes[i];
        kernel_caches[i].slabs_partial = NULL;
        kernel_caches[i].slabs_full = NULL;
    }
}

void *slab_alloc(kmem_cache_t *cache){
    if (!cache) return NULL;

    if (!cache->slabs_partial){
        slab_t *new_slab = slab_create(cache);
        if (!new_slab) return NULL;
        slab_list_add(&cache->slabs_partial, new_slab);
    }

    slab_t *slab = cache->slabs_partial;
    void *obj = slab->free_list;

    slab->free_list = *(void**)obj;
    slab->free_count--;

    if (slab->free_count == 0){
        slab_list_remove(&cache->slabs_partial, slab);
        slab_list_add(&cache->slabs_full, slab);
    }

    return obj;
}

slab_t *slab_create(kmem_cache_t *cache){
    void *page = buddy_alloc(0);
    if (!page) return NULL;

    int index = ((uint64_t)page - HEAP_START) / PAGE_SIZE_4KB;
    metadata[index].is_slab = 1;

    slab_t *slab = (slab_t *)page;
    slab->parent_cache = cache;

    uint64_t first_obj = ((uint64_t)page + sizeof(slab_t) + 7) & ~7;
    uint64_t page_end = (uint64_t)page + PAGE_SIZE_4KB;

    slab->free_list = (void*)first_obj;
    slab->free_count = 0;

    uint64_t curr = first_obj;
    while (curr + cache->obj_size <= page_end){
        uint64_t next = curr + cache->obj_size;

        if (next + cache->obj_size <= page_end) *(uint64_t *)curr = next;
        else *(uint64_t *)curr = 0;

        curr = next;
        slab->free_count++;
    }

    slab->total_count = slab->free_count;
    return slab;
}

void slab_free(void *ptr){
    if (!ptr) return;

    slab_t *slab = (slab_t *)((uint64_t)ptr & PAGE_MASK_4KB);
    kmem_cache_t *cache = slab->parent_cache;

    if (slab->free_count == 0){
        slab_list_remove(&cache->slabs_full, slab);
        slab_list_add(&cache->slabs_partial, slab);
    }

    *(void**)ptr = slab->free_list;
    slab->free_list = ptr;
    slab->free_count++;

    if (slab->free_count == slab->total_count){
        slab_list_remove(&cache->slabs_partial, slab);
        int index = ((uint64_t)ptr - HEAP_START) / PAGE_SIZE_4KB;
        metadata[index].is_slab = 0;

        buddy_free(ptr);
    }
}

void slab_list_remove(slab_t **head, slab_t *slab){
    if (slab->next) slab->next->prev = slab->prev;
    if (slab->prev) slab->prev->next = slab->next;
    else *head = slab->next;
    slab->prev = slab->prev = NULL;
}

void slab_list_add(slab_t **head, slab_t *slab){
    slab->next = *head;
    slab->prev = NULL;

    if (*head) (*head)->prev = slab;

    *head = slab;
}