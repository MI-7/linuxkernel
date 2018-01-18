#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>

struct IDT_PTR {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct IDT {
	uint16_t offset_1;  /* base low */
	uint16_t selector;
	uint8_t  zero;
	uint8_t  type_attr; /* flags */
	uint16_t offset_2;  /* base high */
} __attribute__((packed));

void idt_create_entry(uint8_t entry_num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_initialize(void);

/* defined in idt.s */
extern void idt_load();

#endif
