#pragma once
#include <stdint.h>

#define CEIL_DIV(a,b) (((a + b) - 1)/b)

struct InterruptRegisters {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
}__attribute__((packed));

void memset(void *dest, char val, uint32_t count);
void outb(uint16_t port, uint8_t val);
char inb(uint16_t port);
void invalidate(uint32_t vaddr);
void sti(void);
void cli(void);
void hlt(void);