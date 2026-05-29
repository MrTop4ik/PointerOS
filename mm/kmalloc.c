#include <mm/kmalloc.h>

static spinlock_t kmalloc_lock = {0};

void init_kheap(void){
    for (uint64_t vaddr = HEAP_START; vaddr < HEAP_START + HEAP_SIZE; vaddr += PAGE_SIZE_4KB){
        uint64_t paddr = pmm_alloc_page();
        vmm_map_page(read_cr3(), paddr, vaddr, PAGE_SIZE_4KB, (PTE_PRESENT | PTE_WRITABLE));
    }

    init_buddy();
    init_slab();
}

void *kmalloc(size_t size){
    if (size == 0) return NULL;

    uint64_t rflags;

    if (scheduler) rflags = spin_lock_irqsave(&kmalloc_lock);

    if (size > 2048){
        int order = 0;
        size_t s = PAGE_SIZE_4KB;

        while (s < size){
            s <<= 1;
            order++;
        }

        void *mem = buddy_alloc(order);

        if (scheduler) spin_lock_irqrestore(&kmalloc_lock, rflags);

        return mem;
    }

    for (int i = 0; i < NUM_CACHES; i++){
        if (size <= kernel_caches[i].obj_size){
            void *mem = slab_alloc(&kernel_caches[i]);
            if (scheduler) spin_lock_irqrestore(&kmalloc_lock, rflags);
            return mem;
        }
    }

    if (scheduler) spin_lock_irqrestore(&kmalloc_lock, rflags);
    return NULL;
}

void kfree(void *ptr){
    if (!ptr) return;

    uint64_t rflags;
    if (scheduler) rflags = spin_lock_irqsave(&kmalloc_lock);

    int index = ((uint64_t)ptr - HEAP_START) / PAGE_SIZE_4KB;
    
    if (metadata[index].is_slab) slab_free(ptr);
    else buddy_free(ptr);
    
    if (scheduler) spin_lock_irqrestore(&kmalloc_lock, rflags);
}