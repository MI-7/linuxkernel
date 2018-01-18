#include "kernel.h"
#include "terminal.h"
#include "gdt.h"
#include "test.h"
#include "interrupt.h"
#include "idt.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) 
{
	/* Initialize terminal interface */
	/* Init terminal first so that we can print something while doing
	   other initializations as a test */
	terminal_initialize();

	/* Initialize GDT entries */
	gdt_initialize();

	/* re-map pic vectors */
	pic_initialize();

	/* initialize IDT entries */
	idt_initialize();

	/* test_terminal_output(); */
}
