/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

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

/* screen size */
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

typedef struct {
  char c;
  uint8_t color;
  size_t x;
  size_t y;
} TERMINAL_CHAR;

TERMINAL_CHAR terminal_cache[CACHE_SIZE];
uint16_t ch_in_cache;
uint8_t current_page;
 
void terminal_initialize(void) 
{
	ch_in_cache = 0;
	current_page = 0;

	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

/* cache functions */
void put_char_in_cache(char c, uint8_t color, size_t x, size_t y)
{
        terminal_cache[ch_in_cache].c = c;
	terminal_cache[ch_in_cache].color = color;
	terminal_cache[ch_in_cache].x = x;
	terminal_cache[ch_in_cache].y = y;
	ch_in_cache ++;
}

void load_cache_into_terminal_buffer()
{
	size_t start_row, end_row, index;
	uint16_t i;

	if (terminal_row < VGA_HEIGHT)
	{
		start_row = 0;
		end_row = terminal_row;
	}
	else
	{
		start_row = terminal_row - VGA_HEIGHT + 1;
		end_row = terminal_row;
	}

	for (i = 0; i < ch_in_cache; i++)
	{
		if (terminal_cache[i].y >= start_row && terminal_cache[i].y <= end_row)
		{
			index = (terminal_cache[i].y - start_row) * VGA_WIDTH + terminal_cache[i].x;
			terminal_buffer[index] = vga_entry(terminal_cache[i].c, terminal_cache[i].color);
		}
		
		if (terminal_cache[i].y > end_row)
			return;
	}
}

void remove_first_line_from_cache()
{
	uint16_t to_be_removed = 0, i;
	
	while (terminal_cache[to_be_removed++].y == 0)
		;

	for (i = to_be_removed; i < ch_in_cache; i++)
	{
		terminal_cache[i].y--;
		terminal_cache[i - to_be_removed] = terminal_cache[i];
	}
}
/* cache functions */

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

/*
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
*/

uint8_t c_is_control_characters(char c)
{
	if (c=='\n')
		return true;
	else
		return false;
}

uint8_t c_is_newline_character(char c)
{
	if (c=='\n')
		return true;
	else
		return false;
}

void terminal_putchar(char c) 
{
        if (!c_is_control_characters(c) && !c_is_newline_character(c)) {
		/* put char in cache first */
		put_char_in_cache(c, terminal_color, terminal_column, terminal_row);

		/* terminal_putentryat(c, terminal_color, terminal_column, terminal_row); */
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (terminal_row == VGA_HEIGHT * 3)
				/* remove one line from cache */
				remove_first_line_from_cache();
			else
				terminal_row ++;
		}
	}
	else if (c_is_newline_character(c))
	{
		terminal_column = 0;
		if (terminal_row == VGA_HEIGHT * 3)
		{
			/* remove one line from cache */
			remove_first_line_from_cache();
		}
		else
			terminal_row ++;
	}
}
 
void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
        }
}
 
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
	
	/* transfer cache into terminal buffer */
	load_cache_into_terminal_buffer();
}
 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();
 
	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World1!\n");
	terminal_writestring("Hello, kernel World2!\n");
	terminal_writestring("Hello, kernel World3!\n");
	terminal_writestring("Hello, kernel World4!\n");
	terminal_writestring("Hello, kernel World5!\n");
	terminal_writestring("Hello, kernel World6!\n");
	terminal_writestring("Hello, kernel World7!\n");
	terminal_writestring("Hello, kernel World8!\n");
	terminal_writestring("Hello, kernel World9!\n");
	terminal_writestring("Hello, kernel World10!\n");
	terminal_writestring("Hello, kernel World11!\n");
	terminal_writestring("Hello, kernel World12!\n");
	terminal_writestring("Hello, kernel World13!\n");
	terminal_writestring("Hello, kernel World14!\n");
	terminal_writestring("Hello, kernel World15!\n");
	terminal_writestring("Hello, kernel World16!\n");
	terminal_writestring("Hello, kernel World17!\n");
	terminal_writestring("Hello, kernel World18!\n");
	terminal_writestring("Hello, kernel World19!\n");
	terminal_writestring("Hello, kernel World20!\n");
	terminal_writestring("Hello, kernel World21!\n");
	terminal_writestring("Hello, kernel World22!\n");
	terminal_writestring("Hello, kernel World23!\n");
	terminal_writestring("Hello, kernel World24!\n");
	terminal_writestring("Hello, kernel World25!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26!\n");
	terminal_writestring("Hello, kernel World26+!\n");
	terminal_writestring("Hello, kernel World26++!\n");
	terminal_writestring("Hello, kernel World26+++!\n");
	terminal_writestring("Hello, kernel World26++++!\n");
	terminal_writestring("Hello, kernel World26-!\n");
	terminal_writestring("Hello, kernel World26--!\n");
	terminal_writestring("Hello, kernel World26---!\n");
	terminal_writestring("Hello, kernel World26----!\n");
	terminal_writestring("Hello, kernel World26-----!\n");
	terminal_writestring("Hello, kernel World26------!\n");
	terminal_writestring("Hello, kernel World260!\n");
	terminal_writestring("Hello, kernel World261!\n");
	terminal_writestring("Hello, kernel World262!\n");
	terminal_writestring("Hello, kernel World263!\n");
	terminal_writestring("Hello, kernel World264!\n");
	terminal_writestring("Hello, kernel World265!\n");
	terminal_writestring("Hello, kernel World266!\n");
        terminal_writestring("012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
}
