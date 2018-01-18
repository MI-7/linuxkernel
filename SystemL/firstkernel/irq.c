#include "irq.h"

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

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);
}

