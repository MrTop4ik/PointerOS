#include "memory.h"
#include "vga/vga.h"
#include "utils/utils.h"

static uint32_t pageFrameMin;
static uint32_t pageFrameMax;
static uint32_t totalAlloc;

uint8_t physicalMemoryBitmap[NUM_PAGE_FRAMES / 8];

#define BITMAP_SET(bit) (physicalMemoryBitmap[(bit)/8] |= (1 << ((bit) % 8)))
#define BITMAP_CLR(bit) (physicalMemoryBitmap[(bit)/8] &= ~(1 << ((bit) % 8)))

void init_PMM(uint32_t physicalAllocStart, uint32_t mem_high, struct multiboot_info *bootInfo){
    memset(physicalMemoryBitmap, 0xFF, sizeof(physicalMemoryBitmap));

    if (bootInfo->flags & (1 << 6)){
        struct multiboot_mmap_entry* mmap = (struct multiboot_mmap_entry *)(bootInfo->mmap_addr + KERNEL_START);
        
        while ((uint32_t)mmap < bootInfo->mmap_addr + bootInfo->mmap_length + KERNEL_START){
            if (mmap->size == 0){
                break;
            }

            if (mmap->type == 1){
                uint32_t addr = (uint32_t)mmap->addr_low;
                uint32_t end_addr = addr + (uint32_t)mmap->len_low;
            
                for (; addr < end_addr && addr < mem_high; addr += 0x1000){
                uint32_t frame = addr / 0x1000;
                BITMAP_CLR(frame);
                }
            }
            mmap = (struct multiboot_mmap_entry*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
        }

        BITMAP_SET(0);

        uint32_t kernel_phys_start = 0x100000;
        for (uint32_t addr = kernel_phys_start; addr < physicalAllocStart; addr += 0x1000){
            BITMAP_SET(addr / 0x1000);
        }
        
        pageFrameMin = physicalAllocStart / 0x1000;
        pageFrameMax = mem_high / 0x1000;
        totalAlloc = 0;
    }
}

void init_Memory(uint32_t mem_high, uint32_t physicalAllocStart, struct multiboot_info *bootInfo){
    page_dir[0] = 0;
    invalidate(0);
    page_dir[1023] = ((uint32_t)page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
    invalidate(0xFFFFF000);

    init_PMM(physicalAllocStart, mem_high, bootInfo);
}