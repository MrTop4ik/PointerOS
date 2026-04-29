#include "lfb.h"

void init_LFB(unsigned int physBootInfo){
    struct multiboot_info *virtBootInfo = (struct multiboot_info *)(physBootInfo + KERNEL_OFFSET);

    uint64_t fb_addr = 0;
    uint64_t fb_size = 0;

    struct multiboot_tag* tag = (struct multiboot_tag *)((uint8_t *)virtBootInfo + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END){
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER){
            struct multiboot_tag_framebuffer *fb = (struct multiboot_tag_framebuffer *)tag;
            fb_addr = fb->framebuffer_addr;
            fb_size = fb->framebuffer_pitch * fb->framebuffer_height;
        }
        tag = (struct multiboot_tag *)((uintptr_t)((uint8_t*)tag + tag->size + 7) & ~7);
    }

    for (int size = 0; size < fb_size; size += PAGE_SIZE_2MB){
        vmm_map_page(read_cr3(), fb_addr + size, size + LFB_ADDR, PAGE_SIZE_2MB, (PTE_WRITABLE | PTE_PAT_HUGE));
    }
}