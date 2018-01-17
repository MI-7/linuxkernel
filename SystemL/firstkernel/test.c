#include "test.h"
#include "terminal.h"

void test_terminal_output(void)
{
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
	terminal_writestring("Hello, kernel World26!!\n");
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
	terminal_writestring("Hello, kernel World260!!");
        /*terminal_writestring("012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");*/
	/* transfer cache into terminal buffer */
	load_cache_into_terminal_buffer();
}
