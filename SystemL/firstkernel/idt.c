#include "idt.h"
#include "terminal.h"
#include "io.h"

struct IDT idt[256];
struct IDT_PTR idt_ptr;

/*
	uint16_t offset_1;
	uint16_t selector;
	uint8_t  zero;
	uint8_t  type_attr;
	uint16_t offset_2;
*/
void idt_create_entry(uint8_t entry_num, void* base, uint16_t sel, uint8_t flags)
{
	uint32_t baseinteger = (uint32_t) base;

	idt[entry_num].offset_1 = baseinteger & 0xFFFF;
	idt[entry_num].offset_2 = (baseinteger >> 16) & 0xFFFF;

	idt[entry_num].selector = sel;
	idt[entry_num].zero = 0;
	idt[entry_num].type_attr = flags;
}

/* =================isr================ */
/* This is a simple string array. It contains the message that
*  corresponds to each and every exception. We get the correct
*  message by accessing like:
*  exception_message[interrupt_number] */
char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* This is a very repetitive function... it's not hard, it's
*  just annoying. As you can see, we set the first 32 entries
*  in the IDT to the first 32 ISRs. We can't use a for loop
*  for this, because there is no way to get the function names
*  that correspond to that given entry. We set the access
*  flags to 0x8E. This means that the entry is present, is
*  running in ring 0 (kernel level), and has the lower 5 bits
*  set to the required '14', which is represented by 'E' in
*  hex. */
void isrs_initialize()
{
    idt_create_entry(0, &_isr0, 0x08, 0x8E);
    idt_create_entry(1, &_isr1, 0x08, 0x8E);
    idt_create_entry(2, &_isr2, 0x08, 0x8E);
    idt_create_entry(3, &_isr3, 0x08, 0x8E);
    idt_create_entry(4, &_isr4, 0x08, 0x8E);
    idt_create_entry(5, &_isr5, 0x08, 0x8E);
    idt_create_entry(6, &_isr6, 0x08, 0x8E);
    idt_create_entry(7, &_isr7, 0x08, 0x8E);

    idt_create_entry(8, &_isr8, 0x08, 0x8E);
    idt_create_entry(9, &_isr9, 0x08, 0x8E);
    idt_create_entry(10, &_isr10, 0x08, 0x8E);
    idt_create_entry(11, &_isr11, 0x08, 0x8E);
    idt_create_entry(12, &_isr12, 0x08, 0x8E);
    idt_create_entry(13, &_isr13, 0x08, 0x8E);
    idt_create_entry(14, &_isr14, 0x08, 0x8E);
    idt_create_entry(15, &_isr15, 0x08, 0x8E);

    idt_create_entry(16, &_isr16, 0x08, 0x8E);
    idt_create_entry(17, &_isr17, 0x08, 0x8E);
    idt_create_entry(18, &_isr18, 0x08, 0x8E);
    idt_create_entry(19, &_isr19, 0x08, 0x8E);
    idt_create_entry(20, &_isr20, 0x08, 0x8E);
    idt_create_entry(21, &_isr21, 0x08, 0x8E);
    idt_create_entry(22, &_isr22, 0x08, 0x8E);
    idt_create_entry(23, &_isr23, 0x08, 0x8E);

    idt_create_entry(24, &_isr24, 0x08, 0x8E);
    idt_create_entry(25, &_isr25, 0x08, 0x8E);
    idt_create_entry(26, &_isr26, 0x08, 0x8E);
    idt_create_entry(27, &_isr27, 0x08, 0x8E);
    idt_create_entry(28, &_isr28, 0x08, 0x8E);
    idt_create_entry(29, &_isr29, 0x08, 0x8E);
    idt_create_entry(30, &_isr30, 0x08, 0x8E);
    idt_create_entry(31, &_isr31, 0x08, 0x8E);

    print_something("exception isr initialized\n");
}

/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void fault_handler()
{
	print_something("\nException. System Halted!\n");
	for (;;);
}


/* =================irq================ */
/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)())
{
    irq_routines[irq] = handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers */
void irq_initialize()
{
    /* irq_remap(); do it in pic initialization */

    idt_create_entry(32, &_irq0, 0x08, 0x8E);
    idt_create_entry(33, &_irq1, 0x08, 0x8E);
    idt_create_entry(34, &_irq2, 0x08, 0x8E);
    idt_create_entry(35, &_irq3, 0x08, 0x8E);
    idt_create_entry(36, &_irq4, 0x08, 0x8E);
    idt_create_entry(37, &_irq5, 0x08, 0x8E);
    idt_create_entry(38, &_irq6, 0x08, 0x8E);
    idt_create_entry(39, &_irq7, 0x08, 0x8E);
    idt_create_entry(40, &_irq8, 0x08, 0x8E);
    idt_create_entry(41, &_irq9, 0x08, 0x8E);
    idt_create_entry(42, &_irq10, 0x08, 0x8E);
    idt_create_entry(43, &_irq11, 0x08, 0x8E);
    idt_create_entry(44, &_irq12, 0x08, 0x8E);
    idt_create_entry(45, &_irq13, 0x08, 0x8E);
    idt_create_entry(46, &_irq14, 0x08, 0x8E);
    idt_create_entry(47, &_irq15, 0x08, 0x8E);
    
    print_something("irq initialized\n");
}

/* Each of the IRQ ISRs point to this function, rather than
*  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
void high_irq_handler()
{
    print_something("some high interrupt happened\n");

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    outb(0xA0, 0x20);

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outb(0x20, 0x20);
}

void low_irq_handler()
{
    print_something("some low interrupt happened\n");

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    /* outb(0xA0, 0x20); */

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outb(0x20, 0x20);
}

void keyboard_handler()
{
	uint8_t scancode = inb(0x60);
	print_something("x");
	/* todo: uint8_t character = convert_scancode_ascii(scancode); */
	outb(0x20, 0x20);
}


void idt_initialize(void)
{
	idt_ptr.limit = (sizeof (struct IDT_PTR) * 256) - 1;
	idt_ptr.base = (uint32_t) &idt;

	/* Fill in the remaining entries with a standard isr */
	for(uint16_t i = 48; i < 256; i++)
	{
		idt_create_entry(i, &int_general, 0x08, 0x8E);
	}

	idt_load();

	print_something("idt initialized\n");
}
