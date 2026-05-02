#include "pmm.h"

uint8_t* bitmap;
uint64_t total_pages = 0;

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

void init_PMM(unsigned int bootInfoAddr){
    struct multiboot_info* virtBootInfo = (struct multiboot_info *)(bootInfoAddr + KERNEL_OFFSET);
    uint64_t maxAddr = 0;

    struct multiboot_tag* tag = (struct multiboot_tag *)((uint8_t *)virtBootInfo + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END){
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP){
            struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap *)tag;
            uint64_t num_entries = (mmap->size - sizeof(struct multiboot_tag_mmap)) / mmap->entry_size;
            for (uint64_t i = 0; i < num_entries; i++){
                if (mmap->entries[i].type == MULTIBOOT_MEMORY_AVAILABLE){
                    uint64_t end = mmap->entries[i].addr + mmap->entries[i].len;
                    if (end > maxAddr){
                        maxAddr = end;
                    }
                }
            }
        }
        tag = (struct multiboot_tag *)((uintptr_t)((uint8_t*)tag + tag->size + 7) & ~7);
    }

    total_pages = maxAddr / PAGE_SIZE_4KB;
    uint64_t bitmap_size = (total_pages + 7)/ 8;

    bitmap = (uint8_t *)kernel_end;
    memset(bitmap, 0xFF, bitmap_size);

    tag = (struct multiboot_tag *)((uint8_t *)virtBootInfo + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END){
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP){
            struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap *)tag;
            uint64_t num_entries = (mmap->size - sizeof(struct multiboot_tag_mmap)) / mmap->entry_size;
            for (uint64_t i = 0; i < num_entries; i++){
                if (mmap->entries[i].type == MULTIBOOT_MEMORY_AVAILABLE && mmap->entries[i].addr >= 0x100000){
                    for (uint64_t addr = mmap->entries[i].addr; addr < mmap->entries[i].addr + mmap->entries[i].len; addr += PAGE_SIZE_4KB){
                        BITMAP_CLR(addr/PAGE_SIZE_4KB);
                    }
                }
            }
        }
        tag = (struct multiboot_tag *)((uintptr_t)((uint8_t*)tag + tag->size + 7) & ~7);
    }

    uint64_t phys_kernel_start = (uintptr_t)kernel_start - KERNEL_OFFSET;
    uint64_t phys_kernel_end = ((uintptr_t)bitmap + bitmap_size) - KERNEL_OFFSET;

    for (uint64_t addr = phys_kernel_start; addr < phys_kernel_end; addr += PAGE_SIZE_4KB){
        BITMAP_SET(addr/PAGE_SIZE_4KB);
    }
}

uint64_t pmm_alloc_page(void){
    for (uint64_t i = 0; i < total_pages; i++){
        if (!BITMAP_TEST(i)){
            BITMAP_SET(i);
            return i * PAGE_SIZE_4KB;
        }
    }
}