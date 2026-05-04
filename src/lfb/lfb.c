#include "lfb.h"

framebuffer_t lfb;

void init_LFB(unsigned int physBootInfo){
    struct multiboot_info *virtBootInfo = (struct multiboot_info *)(physBootInfo + KERNEL_OFFSET);

    struct multiboot_tag* tag = (struct multiboot_tag *)((uint8_t *)virtBootInfo + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END){
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER){
            struct multiboot_tag_framebuffer *fb = (struct multiboot_tag_framebuffer *)tag;
            lfb.paddr = fb->framebuffer_addr;
            lfb.width = fb->framebuffer_width;
            lfb.height = fb->framebuffer_height;
            lfb.pitch = fb->framebuffer_pitch;
            lfb.size = lfb.pitch * lfb.height;
        }
        tag = (struct multiboot_tag *)((uintptr_t)((uint8_t*)tag + tag->size + 7) & ~7);
    }

    for (int size = 0; size < lfb.size; size += PAGE_SIZE_2MB){
        vmm_map_page(read_cr3(), lfb.paddr + size, size + LFB_ADDR, PAGE_SIZE_2MB, (PTE_WRITABLE | PTE_PAT_HUGE));
    }

    lfb.vram = (uint32_t *)LFB_ADDR;
    lfb.buffer = kmalloc(lfb.size);
    
    lfb.font = (uint8_t *)font8x16;
    lfb.char_width = 8;
    lfb.char_height = 16;
    lfb.cursor_x = 0;
    lfb.cursor_y = 0;

    memset(lfb.buffer, 0, lfb.size);
    lfb_swap();
}

void lfb_swap(void){
    memcpy(lfb.vram, lfb.buffer, lfb.size);
}

void lfb_scroll(void){
    uint32_t line_size = lfb.pitch * lfb.char_height;
    uint32_t move_size = lfb.size - line_size;

    memcpy(lfb.buffer, (uint8_t *)lfb.buffer + line_size, move_size);
    memset((uint8_t *)lfb.buffer + move_size, 0, line_size);
}

void putpixel(int x, int y, uint32_t color){
    if (x >= lfb.width || y >= lfb.height) return;
    lfb.buffer[y * (lfb.pitch / 4) + x] = color;
}

void draw_char_at(int x, int y, char c, uint32_t fg, uint32_t bg){
    uint8_t *glyph = lfb.font  + (uint8_t)c * lfb.char_height;

    for (int i = 0; i < lfb.char_height; i++){
        for (int j = 0; j < lfb.char_width; j++){
            if ((glyph[i] >> (7 - j)) & 1) putpixel(x + j, y + i, fg);
            else putpixel(x + j, y + i, bg);
        }
    }
}

void kputchar(char c){
    if (c == '\n'){
        lfb.cursor_x = 0;
        lfb.cursor_y += lfb.char_height;
    } else {
        draw_char_at(lfb.cursor_x, lfb.cursor_y, c, 0xFFFFFFFF, 0x0);
        lfb.cursor_x += lfb.char_width;
    }

    if (lfb.cursor_x + lfb.char_width > lfb.width){
        lfb.cursor_x = 0;
        lfb.cursor_y += lfb.char_height;
    }

    if (lfb.cursor_y + lfb.char_height > lfb.height){
        lfb_scroll();
        lfb.cursor_y -= lfb.char_height;
    }
}

void kputnum(uint32_t num, uint32_t base){
    char buf[32];
    int i = 0;
    const char digits[] = "0123456789abcdef";

    if (num == 0) {
        kputchar('0');
        return;
    }

    while (num > 0) {
        buf[i++] = digits[num % base];
        num /= base;
    }

    while (i > 0) {
        kputchar(buf[--i]);
    }
}

void kprintf(const char* format, ...){
    va_list args;
    va_start(args, format);

    for (const char* p = format; *p != '\0'; p++) {
        if (*p != '%') {
            kputchar(*p);
            continue;
        }

        p++;
        switch (*p) {
            case 's':
                kprintf(va_arg(args, char*));
                break;
            case 'd': {
                int i = va_arg(args, int);
                if (i < 0) {
                    kputchar('-');
                    i = -i;
                }
                kputnum((uint32_t)i, 10);
                break;
            }
            case 'x': {
                uint32_t x = va_arg(args, uint32_t);
                kprintf("0x");
                kputnum(x, 16);
                break;
            }
            case 'l':
                if (*(p+1) == 'l' && *(p+2) == 'x') {
                    p += 2;
                    uint64_t val = va_arg(args, uint64_t);

                    kprintf("0x");
                    
                    if (val == 0) {
                        kputchar('0');
                    } else {
                        char buf[16];
                        int i = 0;
                        while (val > 0) {
                            uint32_t rem = val % 16;
                            buf[i++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'a');
                            val /= 16;
                        }
                        while (i > 0) {
                            kputchar(buf[--i]);
                        }
                    }
                }
                break;
            case 'p':
                kprintf("0x");
                kputnum(va_arg(args, uint32_t), 16);
                break;
            case '%':
                kputchar('%');
                break;
        }
    }
    va_end(args);

    lfb_swap();
}