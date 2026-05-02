#include "kmalloc.h"

void init_kheap(void){
    for (uint64_t vaddr = HEAP_START; vaddr < HEAP_START + HEAP_SIZE; vaddr += PAGE_SIZE_4KB){
        uint64_t paddr = pmm_alloc_page();
        vmm_map_page(read_cr3(), paddr, vaddr, PAGE_SIZE_4KB, (PTE_PRESENT | PTE_WRITABLE));
    }

    init_buddy();
}