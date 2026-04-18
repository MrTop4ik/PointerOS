#include "vmm.h"

#define VIRT_OFFSET 0xFFFFFFFF80000000

void init_VMM(unsigned int bootInfoAddr){
    struct multiboot_info* virtBootInfo = (struct multiboot_info *)(bootInfoAddr + VIRT_OFFSET);
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
}