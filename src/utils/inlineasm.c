#include "utils.h"

void outb(uint16_t port, uint8_t val){
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (val));
}

uint8_t inb(uint16_t port){
    uint8_t res;
    __asm__ volatile("inb %1, %0" : "=a" (res) : "dN" (port));
    return res;
}

void invalidate(uint64_t vaddr){
    __asm__ volatile ("invlpg (%0)" : : "r" (vaddr) : "memory");
}

void sti(void){
    __asm__ volatile ("sti");
}

void cli(void){
    __asm__ volatile ("cli");
}

void hlt(void){
    __asm__ volatile ("hlt");
}
