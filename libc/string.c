#include <libc/string.h>

void* memset(void* dest, int char_val, size_t count){
    uint8_t* ptr = (uint8_t*)dest;
    while (count--){
        *ptr++ = (uint8_t)char_val;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, size_t count){
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    while (count--){
        *d++ = *s++;
    }
    return dest;
}