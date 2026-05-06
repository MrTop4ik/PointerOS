#pragma once
#include <stdint.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void invalidate(uint64_t vaddr);
uint64_t read_cr3(void);
void write_cr3(uint64_t paddr);
void sti(void);
void cli(void);
void hlt(void);
