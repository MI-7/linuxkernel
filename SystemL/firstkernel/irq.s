.intel_syntax noprefix

.section .text

.global int_general
.global _isr0
.global _isr1
.global _isr2
.global _isr3
.global _isr4
.global _isr5
.global _isr6
.global _isr7
.global _isr8
.global _isr9
.global _isr10
.global _isr11
.global _isr12
.global _isr13
.global _isr14
.global _isr15
.global _isr16
.global _isr17
.global _isr18
.global _isr19
.global _isr20
.global _isr21
.global _isr22
.global _isr23
.global _isr24
.global _isr25
.global _isr26
.global _isr27
.global _isr28
.global _isr29
.global _isr30
.global _isr31

.global _irq0
.global _irq1
.global _irq2
.global _irq3
.global _irq4
.global _irq5
.global _irq6
.global _irq7
.global _irq8
.global _irq9
.global _irq10
.global _irq11
.global _irq12
.global _irq13
.global _irq14
.global _irq15

/* We call a C function in here. We need to let the assembler know
 that 'irq_handler' exists in another file */
.extern low_irq_handler
.extern high_irq_handler

/* We call a C function in here. We need to let the assembler know
 that 'fault_handler' exists in another file */
.extern fault_handler

.extern keyboard_handler

_irq0:
pusha
push ds
push es
push fs
push gs

	call low_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* keyboard */
_irq1:
pusha
push ds
push es
push fs
push gs

	call keyboard_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq2:
pusha
push ds
push es
push fs
push gs

	call low_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq3:
pusha
push ds
push es
push fs
push gs

	call low_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq4:
pusha
push ds
push es
push fs
push gs

	call low_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq5:
pusha
push ds
push es
push fs
push gs

	call low_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq6:
pusha
push ds
push es
push fs
push gs

	call low_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq7:
pusha
push ds
push es
push fs
push gs

	call low_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq8:
pusha
push ds
push es
push fs
push gs

	call high_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq9:
pusha
push ds
push es
push fs
push gs

	call high_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq10:
pusha
push ds
push es
push fs
push gs

	call high_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq11:
pusha
push ds
push es
push fs
push gs

	call high_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq12:
pusha
push ds
push es
push fs
push gs

	call high_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq13:
pusha
push ds
push es
push fs
push gs

	call high_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq14:
pusha
push ds
push es
push fs
push gs

	call high_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret


_irq15:
pusha
push ds
push es
push fs
push gs

	call high_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret







/* general handler */
int_general:
pusha
push ds
push es
push fs
push gs

	call high_irq_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  0: Divide By Zero Exception */
_isr0:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  1: Debug Exception */
_isr1:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  2: Non Maskable Interrupt Exception */
_isr2:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  3: Int 3 Exception */
_isr3:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  4: INTO Exception */
_isr4:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  5: Out of Bounds Exception */
_isr5:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  6: Invalid Opcode Exception */
_isr6:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  7: Coprocessor Not Available Exception */
_isr7:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  8: Double Fault Exception (With Error Code!) */
_isr8:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/*  9: Coprocessor Segment Overrun Exception */
_isr9:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 10: Bad TSS Exception (With Error Code!) */
_isr10:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 11: Segment Not Present Exception (With Error Code!) */
_isr11:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 12: Stack Fault Exception (With Error Code!) */
_isr12:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 13: General Protection Fault Exception (With Error Code!) */
_isr13:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 14: Page Fault Exception (With Error Code!) */
_isr14:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 15: Reserved Exception */
_isr15:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 16: Floating Point Exception */
_isr16:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 17: Alignment Check Exception */
_isr17:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 18: Machine Check Exception */
_isr18:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 19: Reserved */
_isr19:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 20: Reserved */
_isr20:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 21: Reserved */
_isr21:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 22: Reserved */
_isr22:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 23: Reserved */
_isr23:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 24: Reserved */
_isr24:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 25: Reserved */
_isr25:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 26: Reserved */
_isr26:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 27: Reserved */
_isr27:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 28: Reserved */
_isr28:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 29: Reserved */
_isr29:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 30: Reserved */
_isr30:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret

/* 31: Reserved */
_isr31:
pusha
push ds
push es
push fs
push gs

	call fault_handler

pop gs
pop fs
pop es
pop ds
popa
iret


