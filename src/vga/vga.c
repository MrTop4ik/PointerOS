#include <stdarg.h>
#include "vga.h"
#include "../utils/utils.h"

uint16_t* vga_buffer = (uint16_t*)0xC00B8000;
size_t term_col;
size_t term_row;
uint8_t term_color;

int strlen(char *s){
    int len = 0;
    for (int i = 0; s[i] != '\0'; i++){
        len++;
    }
    return len;
}

char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    if ( base < 2 || base > 36 ){
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    if ( value < 0 && base == 10 ){
        *ptr++ = '-';
    }
    low = ptr;
    do{
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    *ptr-- = '\0';
    while ( low < ptr ){
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

void scrollUp(void){
    for (size_t y = 1; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH; x++){
            vga_buffer[(y-1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++){
        vga_buffer[(VGA_HEIGHT-1) * VGA_WIDTH + x] = (uint16_t)' ' | (uint16_t)term_color << 8;
    }

    term_row = VGA_HEIGHT - 1;
}

void term_init(void){
    term_col = 0;
    term_row = 0;
    term_color = VGA_COLOR_WHITE | (VGA_COLOR_BLACK << 4);

    for (size_t x = 0; x < VGA_WIDTH; x++){
        for (size_t y = 0; y < VGA_HEIGHT; y++){
            const size_t index = VGA_WIDTH * y + x;
            vga_buffer[index] = ' ' | (term_color << 8);
        }
    }
}

void term_putchar(char c){
    switch (c){
        case '\n':
            term_col = 0;
            term_row++; 
            break;
        case '\r':
            term_col = 0;
            break;
        case '\t':
            if (term_col == VGA_WIDTH){
                term_col = 0;
                term_row++;
            }
            uint16_t tabLen = 4 - (term_col % 4);
            while (tabLen != 0){
                vga_buffer[VGA_WIDTH * term_row + (term_col++)] = (uint16_t)' ' | (uint16_t)term_color << 8;
                tabLen--;
            }
            break;
        case '\b':
            if (term_col > 0){
                term_col--;
            } else if (term_row > 0){
                term_row--;
                term_col = VGA_WIDTH - 1;
            } else {
                break;
            }
            vga_buffer[VGA_WIDTH * term_row + term_col] = (uint16_t)' ' | (uint16_t)term_color << 8;
            break;
        default:
            const size_t index = (VGA_WIDTH * term_row) + term_col;
            vga_buffer[index] = (uint16_t)c | (uint16_t)term_color << 8;

            term_col++;
            break;
    }

    if (term_col >= VGA_WIDTH){
		term_col = 0;
		term_row ++;
	}
	
	if (term_row >= VGA_HEIGHT){
        scrollUp();
        term_col = 0;
	}

    term_update_cursor();
}

void term_put_number(uint32_t num, uint32_t base){
    char buf[32];
    int i = 0;
    const char digits[] = "0123456789abcdef";

    if (num == 0) {
        term_putchar('0');
        return;
    }

    while (num > 0) {
        buf[i++] = digits[num % base];
        num /= base;
    }

    while (i > 0) {
        term_putchar(buf[--i]);
    }
}

void term_print(const char* format, ...){
    va_list args;
    va_start(args, format);

    for (const char* p = format; *p != '\0'; p++) {
        if (*p != '%') {
            term_putchar(*p);
            continue;
        }

        p++;
        switch (*p) {
            case 's':
                term_print(va_arg(args, char*));
                break;
            case 'd': {
                int i = va_arg(args, int);
                if (i < 0) {
                    term_putchar('-');
                    i = -i;
                }
                term_put_number((uint32_t)i, 10);
                break;
            }
            case 'x': {
                uint32_t x = va_arg(args, uint32_t);
                term_put_number(x, 16);
                break;
            }
            case 'p':
                term_print("0x");
                term_put_number(va_arg(args, uint32_t), 16);
                break;
            case '%':
                term_putchar('%');
                break;
        }
    }
    va_end(args);
}

void term_update_cursor(void){
    uint16_t cursor_pos = term_row * VGA_WIDTH  + term_col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(cursor_pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((cursor_pos >> 8) & 0xFF));
}