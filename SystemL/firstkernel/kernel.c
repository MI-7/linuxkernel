#include "kernel.h"

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

	/* create entries for ISR for exceptions */
	/* isrs_initialize(); moved into idt_initialize() */

	/* create irq handlers */
	/* irq_initialize(); moved into idt_initialize() */

	test_terminal_output();

	/* not necessary, the boot.s already did */
	/* while(1)
		__asm__("hlt\n\t"); */
}
