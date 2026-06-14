#pragma once
#include <stdint.h>
#include <multiboot2.h>
#include <libc/string.h>
#include <arch/x86_64/drivers/video/serial.h>
#include <kernel/spinlock.h>

#define KERNEL_OFFSET 0xFFFFFFFF80000000
#define PAGE_SIZE_4KB 0x1000

#define BITMAP_SET(bit)  (bitmap[(bit)/8] |= (1 << ((bit) % 8)))
#define BITMAP_CLR(bit)  (bitmap[(bit)/8] &= ~(1 << ((bit) % 8)))
#define BITMAP_TEST(bit) (bitmap[(bit)/8] & (1 << ((bit)% 8)))

void init_PMM(unsigned int bootInfoAddr);
uint64_t pmm_alloc_page(void);
void pmm_free_page(uint64_t ptr);