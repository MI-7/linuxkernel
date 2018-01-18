#include "terminal.h"
#include "utility.h"

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
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
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
	/* terminal_cache[ch_in_cache].color = color; */
	/* no black front color */
	terminal_cache[ch_in_cache].color = vga_entry_color((x+y) % 14 + 1, VGA_COLOR_BLACK);
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

	/* initialize / clear the terminal buffer */
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
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
	
	while (terminal_cache[to_be_removed].y == 0)
		to_be_removed++;

	for (i = to_be_removed; i < ch_in_cache; i++)
	{
		terminal_cache[i].y--;
		terminal_cache[i - to_be_removed] = terminal_cache[i];
	}

	ch_in_cache = ch_in_cache - to_be_removed;
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
			if (terminal_row == VGA_HEIGHT * 3 - 1)
				/* remove one line from cache */
				remove_first_line_from_cache();
			else
				terminal_row ++;
		}
	}
	else if (c_is_newline_character(c))
	{
		terminal_column = 0;
		if (terminal_row == VGA_HEIGHT * 3 - 1)
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
}

void print_something(const char* data)
{
	terminal_writestring(data);
	load_cache_into_terminal_buffer();
}
