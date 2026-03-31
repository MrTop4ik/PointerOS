#include <stdint.h>

#define KERNEL_START            0xC0000000
#define PAGE_FLAG_PRESENT       (1 << 0)
#define PAGE_FLAG_WRITE         (1 << 1)
#define NUM_KERNEL_PAGE_TABLES  256
#define NUM_PAGE_FRAMES         (0x100000000 / 0x1000 / 8)

extern uint32_t page_dir[1024];

void pmm_init(uint32_t mem_low, uint32_t mem_high);
void init_Memory(uint32_t mem_high, uint32_t physicalAllocStart);