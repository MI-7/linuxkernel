#include "idt.h"
#include "terminal.h"

struct IDT idt[256];
struct IDT_PTR idt_ptr;

/*
	uint16_t offset_1;
	uint16_t selector;
	uint8_t  zero;
	uint8_t  type_attr;
	uint16_t offset_2;
*/
void idt_create_entry(uint8_t entry_num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt[entry_num].offset_1 = base & 0xFFFF;
	idt[entry_num].offset_2 = (base >> 16) & 0xFFFF;

	idt[entry_num].selector = sel;
	idt[entry_num].zero = 0;
	idt[entry_num].type_attr = flags;
}

void idt_initialize(void)
{
	idt_ptr.limit = (uint16_t) ((sizeof (struct IDT_PTR) * 256) - 1);
	idt_ptr.base = (uint32_t) &idt;

	for (int i = 0; i < 256; i++)
	{
		idt[i].offset_1 = 0;
		idt[i].selector = 0;
		idt[i].zero = 0;
		idt[i].type_attr = 0;
		idt[i].offset_2 = 0;
	}

	idt_load();

	print_something("idt initialized\n");
}
