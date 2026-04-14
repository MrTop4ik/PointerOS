#pragma once
#include <stdint.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void invalidate(uint64_t vaddr);
void sti(void);
void cli(void);
void hlt(void);
