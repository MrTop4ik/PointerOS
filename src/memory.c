#include "memory.h"
#include "vga/vga.h"
#include "utils/utils.h"

static uint32_t pageFrameMin;
static uint32_t pageFrameMax;
static uint32_t totalAlloc;

uint8_t psysicalmemoryBitmap[NUM_PAGE_FRAMES / 8];

static uint32_t pageDirs[NUM_PAGE_DIRS][1024]__attribute__((aligned(1024)));
static uint8_t pageDirsUsed[NUM_PAGE_DIRS];

void init_PMM(uint32_t mem_low, uint32_t mem_high){
    pageFrameMin = CEIL_DIV(mem_low, 0x1000);
    pageFrameMax = CEIL_DIV(mem_high, 0x1000);
    totalAlloc = 0;

    memset(psysicalmemoryBitmap, 0, sizeof(psysicalmemoryBitmap));
}

void init_Memory(uint32_t mem_high, uint32_t psysicalAllocStart){
    page_dir[0] = 0;
    invalidate(0);
    page_dir[1023] = ((uint32_t)page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
    invalidate(0xFFFFF000);

    init_PMM(psysicalAllocStart, mem_high);
    memset(pageDirs, 0, 0x1000 * NUM_PAGE_DIRS);
    memset(pageDirsUsed, 0, NUM_PAGE_DIRS);
}