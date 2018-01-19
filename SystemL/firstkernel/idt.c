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
    idt_create_entry(0, (unsigned)_isr0, 0x08, 0x8E);
    idt_create_entry(1, (unsigned)_isr1, 0x08, 0x8E);
    idt_create_entry(2, (unsigned)_isr2, 0x08, 0x8E);
    idt_create_entry(3, (unsigned)_isr3, 0x08, 0x8E);
    idt_create_entry(4, (unsigned)_isr4, 0x08, 0x8E);
    idt_create_entry(5, (unsigned)_isr5, 0x08, 0x8E);
    idt_create_entry(6, (unsigned)_isr6, 0x08, 0x8E);
    idt_create_entry(7, (unsigned)_isr7, 0x08, 0x8E);

    idt_create_entry(8, (unsigned)_isr8, 0x08, 0x8E);
    idt_create_entry(9, (unsigned)_isr9, 0x08, 0x8E);
    idt_create_entry(10, (unsigned)_isr10, 0x08, 0x8E);
    idt_create_entry(11, (unsigned)_isr11, 0x08, 0x8E);
    idt_create_entry(12, (unsigned)_isr12, 0x08, 0x8E);
    idt_create_entry(13, (unsigned)_isr13, 0x08, 0x8E);
    idt_create_entry(14, (unsigned)_isr14, 0x08, 0x8E);
    idt_create_entry(15, (unsigned)_isr15, 0x08, 0x8E);

    idt_create_entry(16, (unsigned)_isr16, 0x08, 0x8E);
    idt_create_entry(17, (unsigned)_isr17, 0x08, 0x8E);
    idt_create_entry(18, (unsigned)_isr18, 0x08, 0x8E);
    idt_create_entry(19, (unsigned)_isr19, 0x08, 0x8E);
    idt_create_entry(20, (unsigned)_isr20, 0x08, 0x8E);
    idt_create_entry(21, (unsigned)_isr21, 0x08, 0x8E);
    idt_create_entry(22, (unsigned)_isr22, 0x08, 0x8E);
    idt_create_entry(23, (unsigned)_isr23, 0x08, 0x8E);

    idt_create_entry(24, (unsigned)_isr24, 0x08, 0x8E);
    idt_create_entry(25, (unsigned)_isr25, 0x08, 0x8E);
    idt_create_entry(26, (unsigned)_isr26, 0x08, 0x8E);
    idt_create_entry(27, (unsigned)_isr27, 0x08, 0x8E);
    idt_create_entry(28, (unsigned)_isr28, 0x08, 0x8E);
    idt_create_entry(29, (unsigned)_isr29, 0x08, 0x8E);
    idt_create_entry(30, (unsigned)_isr30, 0x08, 0x8E);
    idt_create_entry(31, (unsigned)_isr31, 0x08, 0x8E);

    print_something("exception isr initialized\n");
}

/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
        print_something(exception_messages[r->int_no]);
        print_something("\nException. System Halted!\n");
        for (;;);
    }
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
void irq_install_handler(int irq, void (*handler)(struct regs *r))
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

    idt_create_entry(32, (unsigned)_irq0, 0x08, 0x8E);
    idt_create_entry(33, (unsigned)_irq1, 0x08, 0x8E);
    idt_create_entry(34, (unsigned)_irq2, 0x08, 0x8E);
    idt_create_entry(35, (unsigned)_irq3, 0x08, 0x8E);
    idt_create_entry(36, (unsigned)_irq4, 0x08, 0x8E);
    idt_create_entry(37, (unsigned)_irq5, 0x08, 0x8E);
    idt_create_entry(38, (unsigned)_irq6, 0x08, 0x8E);
    idt_create_entry(39, (unsigned)_irq7, 0x08, 0x8E);
    idt_create_entry(40, (unsigned)_irq8, 0x08, 0x8E);
    idt_create_entry(41, (unsigned)_irq9, 0x08, 0x8E);
    idt_create_entry(42, (unsigned)_irq10, 0x08, 0x8E);
    idt_create_entry(43, (unsigned)_irq11, 0x08, 0x8E);
    idt_create_entry(44, (unsigned)_irq12, 0x08, 0x8E);
    idt_create_entry(45, (unsigned)_irq13, 0x08, 0x8E);
    idt_create_entry(46, (unsigned)_irq14, 0x08, 0x8E);
    idt_create_entry(47, (unsigned)_irq15, 0x08, 0x8E);
    
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
void irq_handler(struct regs *r)
{
    /* This is a blank function pointer */
    void (*handler)(struct regs *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    print_something("some interrupt happened\n");

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outb(0x20, 0x20);
}


void idt_initialize(void)
{
	idt_ptr.limit = (sizeof (struct IDT_PTR) * 256) - 1;
	idt_ptr.base = (uint32_t) &idt;

	for(uint8_t i = 48; i < 256; i++)				// Fill in the remaining entries with a standard isr
	{
		idt_create_entry(i, &int_general, 0x08, 0x8E);
	}

	idt_load();

	print_something("idt initialized\n");
}
