#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_COLOR_BLACK         0
#define VGA_COLOR_BLUE          1
#define VGA_COLOR_GREEN         2
#define VGA_COLOR_CYAN          3
#define VGA_COLOR_RED           4
#define VGA_COLOR_MAGENTA       5
#define VGA_COLOR_BROWN         6
#define VGA_COLOR_LIGHT_GREY    7
#define VGA_COLOR_DARK_GREY     8
#define VGA_COLOR_LIGHT_BLUE    9
#define VGA_COLOR_LIGHT_GREEN   10
#define VGA_COLOR_LIGHT_CYAN    11
#define VGA_COLOR_LIGHT_RED     12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN   14
#define VGA_COLOR_WHITE         15

#define VGA_WIDTH               80
#define VGA_HEIGHT              25

extern uint16_t* vga_buffer;
extern size_t term_col;
extern size_t term_row;
extern uint8_t term_color;

int strlen(char *s);
char * itoa(int value, char * str, int base);
void scrollUp(void);
void term_init(void);
void term_putchar(char);
void term_put_number(uint32_t num, uint32_t base);
void term_print(const char* format, ...);
void term_update_cursor(void);