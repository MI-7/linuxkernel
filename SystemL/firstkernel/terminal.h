#ifndef TERMINAL_H
#define TERMINAL_H
/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

/* character cache size */
#define CACHE_SIZE 6000 /* 80 * 25 * 3 */

#define true  1
#define false 0

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/* cache functions */
void put_char_in_cache(char c, uint8_t color, size_t x, size_t y);

void load_cache_into_terminal_buffer();

void remove_first_line_from_cache();
/* cache functions */

/* terminal functions */
void terminal_setcolor(uint8_t color);

uint8_t c_is_control_characters(char c);

uint8_t c_is_newline_character(char c);

void terminal_putchar(char c);
 
void terminal_write(const char* data, size_t size);
 
void terminal_writestring(const char* data);

void terminal_initialize(void);

void print_something(void);
/* terminal functions */
#endif
