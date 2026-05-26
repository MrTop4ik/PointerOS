#pragma once
#include <stdint.h>
#include <multiboot2.h>
#include <arch/x86_64/inlineasm.h>
#include <libc/string.h>
#include <arch/x86_64/drivers/video/serial.h>
#include <mm/kmalloc.h>
#include <drivers/font8x16.h>
#include <kernel/spinlock.h>

#define LFB_ADDR 0xFFFFFFFFC0000000

typedef struct framebuffer_struct {
    uint32_t *vram;
    uint32_t *buffer;
    uint32_t paddr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t size;
    uint8_t *font;
    uint8_t char_width;
    uint8_t char_height;
    uint32_t cursor_x;
    uint32_t cursor_y;
} framebuffer_t;

void init_LFB(unsigned int physBootInfo);
void lfb_swap(void);
void lfb_scroll(void);
void putpixel(int x, int y, uint32_t color);
void draw_char_at(int x, int y, char c, uint32_t fb, uint32_t bg);
void kputchar_direct(char c);
void kputchar(char c);
void kputnum(uint32_t num, uint32_t base);
void kprintf(const char* format, ...);