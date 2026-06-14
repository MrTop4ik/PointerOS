#include <mm/stack.h>

uint64_t user_next_top = USER_STACK_MAX;

void *kernel_alloc_stack(size_t size){
    if (size == 0) size = DEFAULT_STACK_SIZE;

    void *stack_bottom = kmalloc(size);
    if (!stack_bottom) return NULL;

    return stack_bottom;
}

void *user_alloc_stack(size_t size){
    size = (size + PAGE_SIZE_4KB - 1) & PAGE_MASK_4KB;
    void *stack_bottom = NULL;

    for (size_t i = 0; i < size; i += PAGE_SIZE_4KB){
        uint64_t paddr = pmm_alloc_page();
        user_next_top -= PAGE_SIZE_4KB;
        vmm_map_page(read_cr3(), paddr, user_next_top, PAGE_SIZE_4KB, (PTE_WRITABLE | PTE_USER));
        stack_bottom = (void *)user_next_top;
    }

    return stack_bottom;
}

void user_free_stack(void *stack_bottom, size_t size){
    size = (size + PAGE_SIZE_4KB - 1) & ~(PAGE_SIZE_4KB - 1);

    uintptr_t addr = (uintptr_t)stack_bottom;

    for (size_t i = 0; i < size; i += PAGE_SIZE_4KB){
        uint64_t paddr = vmm_unmap_page(read_cr3(), addr + i);
        if (paddr) pmm_free_page(paddr);
    }
}
