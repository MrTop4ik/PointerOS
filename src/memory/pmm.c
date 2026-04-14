#include "pmm.h"

#define VIRT_OFFSET 0xFFFFFFFF80000000
#define PAGE_SIZE   4096

#define BITMAP_SET(bit)  (bitmap[(bit)/8] |= (1 << ((bit) % 8)))
#define BITMAP_CLR(bit)  (bitmap[(bit)/8] &= ~(1 << ((bit) % 8)))
#define BITMAP_TEST(bit) (bitmap[(bit)/8] & (1 << ((bit)% 8)))

uint8_t* bitmap;
uint64_t total_pages = 0;

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

void init_PMM(unsigned int bootInfoAddr){
    struct multiboot_info* virtBootInfo = (struct multiboot_info *)(bootInfoAddr + VIRT_OFFSET);
    uint64_t maxAddr = 0;

    struct multiboot_tag* tag = (struct multiboot_tag *)((uint8_t *)virtBootInfo + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END){
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP){
            struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap *)tag;
            uint64_t num_entries = (mmap->size - sizeof(struct multiboot_tag_mmap)) / mmap->entry_size;
            for (uint64_t i = 0; i < num_entries; i++){
                serial_print("Addr: %llx | Len: %llx | Type: %d\n", mmap->entries[i].addr, mmap->entries[i].len, mmap->entries[i].type);
                if (mmap->entries[i].type == MULTIBOOT_MEMORY_AVAILABLE){
                    uint64_t end = mmap->entries[i].addr + mmap->entries[i].len;
                    serial_print("end: %llx\n", end);
                    if (end > maxAddr){
                        maxAddr = end;
                    }
                }
            }
        }
        tag = (struct multiboot_tag *)((uintptr_t)((uint8_t*)tag + tag->size + 7) & ~7);
    }

    total_pages = maxAddr / PAGE_SIZE;
    uint64_t bitmap_size = (total_pages + 7)/ 8;

    bitmap = (uint8_t *)kernel_end;
    memset(bitmap, 0xFF, bitmap_size);

    serial_print("maxAddr: %llx | bitmap addr: %llx | bitmap size: %llx\n", maxAddr, (uint64_t)bitmap, bitmap_size);
}