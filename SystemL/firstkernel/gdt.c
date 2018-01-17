#include "gdt.h"
#include "terminal.h"

void gdt_create_entries(uint8_t entry_num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran)
{
	gdt[entry_num].limit_low = limit & 0xFFFF;

	gdt[entry_num].base_low = base & 0xFFFF;
	gdt[entry_num].base_middle = (base >> 16) & 0xFF;
	gdt[entry_num].base_high = (base >> 24) & 0xFF;

	gdt[entry_num].granularity = (limit >> 16) & 0x0F;
	gdt[entry_num].granularity |= (gran & 0xF0);

	gdt[entry_num].access = access;
}

void gdt_initialize()
{
	gdt_ptr.limit = (sizeof(struct GDT) * 3) - 1;
	gdt_ptr.base = (uint32_t) &gdt;

	gdt_create_entries(0, 0, 0, 0, 0);
	gdt_create_entries(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_create_entries(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	print_something();

	/* defined in gdt.s */
	_gdt_flush();
}
