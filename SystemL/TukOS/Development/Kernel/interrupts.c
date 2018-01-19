#include "interrupts.h"
#include "memory.h"
#include "video.h"
#include "common.h"
#include "keyboard.h"

static struct idtstruct idttable[256];	// Define the idt struct

unsigned char* idt_init()
{
	unsigned int i;

	printstring((unsigned char*)"Initialising interrupts... ");

	remappic();

	struct idtpointer idt;			// Contains the struct being loaded by lidt

	idt_entry(0, &int_0, 0x08, 0x8E);	// Flag indicates an interrupt gate
	idt_entry(1, &int_1, 0x08, 0x8E);
	idt_entry(2, &int_2, 0x08, 0x8E);
	idt_entry(3, &int_3, 0x08, 0x8E);
	idt_entry(4, &int_4, 0x08, 0x8E);
	idt_entry(5, &int_5, 0x08, 0x8E);
	idt_entry(6, &int_6, 0x08, 0x8E);
	idt_entry(7, &int_7, 0x08, 0x8E);
	idt_entry(8, &int_8, 0x08, 0x8E);
	idt_entry(9, &int_9, 0x08, 0x8E);
	idt_entry(10, &int_10, 0x08, 0x8E);
	idt_entry(11, &int_11, 0x08, 0x8E);
	idt_entry(12, &int_12, 0x08, 0x8E);
	idt_entry(13, &int_13, 0x08, 0x8E);
	idt_entry(14, &int_14, 0x08, 0x8E);
	idt_entry(15, &int_15, 0x08, 0x8E);
	idt_entry(16, &int_16, 0x08, 0x8E);
	idt_entry(17, &int_17, 0x08, 0x8E);
	idt_entry(18, &int_18, 0x08, 0x8E);
	idt_entry(19, &int_19, 0x08, 0x8E);
	idt_entry(20, &int_20, 0x08, 0x8E);
	idt_entry(21, &int_21, 0x08, 0x8E);
	idt_entry(22, &int_22, 0x08, 0x8E);
	idt_entry(23, &int_23, 0x08, 0x8E);
	idt_entry(24, &int_24, 0x08, 0x8E);
	idt_entry(25, &int_25, 0x08, 0x8E);
	idt_entry(26, &int_26, 0x08, 0x8E);
	idt_entry(27, &int_27, 0x08, 0x8E);
	idt_entry(28, &int_28, 0x08, 0x8E);
	idt_entry(29, &int_29, 0x08, 0x8E);
	idt_entry(30, &int_30, 0x08, 0x8E);
	idt_entry(31, &int_31, 0x08, 0x8E);
	idt_entry(32, &int_32, 0x08, 0x8E);
	idt_entry(33, &int_33, 0x08, 0x8E);	

	for(i = 34; i < 256; i++)				// Fill in the remaining entries with a standard isr
	{
		idt_entry(i, &int_general, 0x08, 0x8E);
	}

	idt.size = sizeof(idttable) - 1;		// Size of the IDT
	idt.offset = (unsigned int)&idttable;		// Pointer to the IDT

	asm("lidt (%0)" : : "p"(&idt));			// Load the IDT struct
	asm("sti" : :);					// Activate interrupts

	printstring((unsigned char*)"Succes\n");
return 0;
}

unsigned int idt_entry(unsigned int entry, void* offset, unsigned short selector, unsigned char flag)
{
	unsigned int offsetinteger = (unsigned int)offset;	// Store pointer into an integer for later processing	

	idttable[entry].offset1 = offsetinteger & 0xFFFF;		
	idttable[entry].selector = selector;
	idttable[entry].unused = 0;
	idttable[entry].flag = flag;
	idttable[entry].offset2 = (offsetinteger >> 16) & 0xFFFF;
return 0;
}

void isr_general()
{
	printstring((unsigned char*)"Interrupt occured\n");
	outport(0x20, 0x20);
}

void isr_33()				// Keyboard handler	
{
	unsigned char scancode = inport(0x60);

	//printchar(convert_scancode_ascii(scancode));

	unsigned char character = convert_scancode_ascii(scancode);

	keyboardbuffer(1, character);
	
	outport(0x20, 0x20);
}

unsigned int remappic()
{
	outport(0x20, 0x11);		// Initilisation instruction
	outport(0xA0, 0x11);
		
	outport(0x21, 0x20);		// Map the first 8 interrupts to 0x20
	outport(0xA1, 0x28);		// Map 8 - 15 interrupts to 0x28

	outport(0x21, 0x04);		// Tell the pic how its connected 
	outport(0xA1, 0x02);		
	
	outport(0x21, 0x01);		// Tell the mode it is operating in
	outport(0xA1, 0x01);

	outport(0x21, 0xfd);
	outport(0xA1, 0xff);

return 0;
}
