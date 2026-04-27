#include "vmm.h"

#define KERNEL_OFFSET 0xFFFFFFFF80000000
#define DIRECT_OFFSET 0xFFFF800000000000

#define PAGE_SIZE_4KB 4096
#define PAGE_MASK_4KB (~(PAGE_SIZE_4KB - 1))
#define PAGE_SIZE_2MB 0x200000
#define PAGE_MASK_2MB (~(PAGE_SIZE_2MB - 1))

#define PTE_PRESENT   (1ULL << 0)
#define PTE_WRITABLE  (1ULL << 1)
#define PTE_USER      (1ULL << 2)
#define PTE_PAGE_SIZE (1ULL << 7)

void init_VMM(unsigned int bootInfoAddr){
    struct multiboot_info* virtBootInfo = (struct multiboot_info *)(bootInfoAddr + KERNEL_OFFSET);
    uint64_t maxAddr = 0;

    struct multiboot_tag* tag = (struct multiboot_tag *)((uint8_t *)virtBootInfo + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END){
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP){
            struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap *)tag;
            uint64_t num_entries = (mmap->size - sizeof(struct multiboot_tag_mmap)) / mmap->entry_size;
            for (uint64_t i = 0; i < num_entries; i++){
                uint64_t end = mmap->entries[i].addr + mmap->entries[i].len;
                if (end > maxAddr){
                    maxAddr = end;
                }
            }
        }
        tag = (struct multiboot_tag *)((uintptr_t)((uint8_t*)tag + tag->size + 7) & ~7);
    }

    vmm_init_direct_mapping(maxAddr);
}

void vmm_init_direct_mapping(uint64_t maxAddr){
    uint64_t pml4_phys = pmm_alloc_page();
    uint64_t *pml4 = (uint64_t *)(pml4_phys + KERNEL_OFFSET);
    memset(pml4, 0, PAGE_SIZE_4KB);

    uint64_t* old_pml4 = (uint64_t *)(read_cr3() + KERNEL_OFFSET);
    pml4[511] = old_pml4[511];

    for (uint64_t paddr = 0; paddr < maxAddr; paddr += PAGE_SIZE_2MB){
        uint64_t vaddr = paddr + DIRECT_OFFSET;

        uint64_t pml4_indx = (vaddr >> 39) & 0x1FF;
        uint64_t pdpt_indx = (vaddr >> 30) & 0x1FF;
        uint64_t pd_indx = (vaddr >> 21) & 0x1FF;

        if (!(pml4[pml4_indx] & PTE_PRESENT)){
            uint64_t new_table_phys = pmm_alloc_page();
            uint64_t *new_table = (uint64_t *)(new_table_phys + KERNEL_OFFSET);
            memset(new_table, 0, PAGE_SIZE_4KB);
            pml4[pml4_indx] = new_table_phys | (PTE_PRESENT | PTE_WRITABLE);
        }

        uint64_t *pdpt = (uint64_t *)((pml4[pml4_indx] & PAGE_MASK_4KB) + KERNEL_OFFSET);

        if (!(pdpt[pdpt_indx] & PTE_PRESENT)){
            uint64_t new_table_phys = pmm_alloc_page();
            uint64_t *new_table = (uint64_t *)(new_table_phys + KERNEL_OFFSET);
            memset(new_table, 0, PAGE_SIZE_4KB);
            pdpt[pdpt_indx] = new_table_phys | (PTE_PRESENT | PTE_WRITABLE);
        }

        uint64_t *pd = (uint64_t *)((pdpt[pdpt_indx] & PAGE_MASK_4KB) + KERNEL_OFFSET);

        pd[pd_indx] = paddr | (PTE_PRESENT | PTE_WRITABLE | PTE_PAGE_SIZE);
    }

    write_cr3(pml4_phys);
}