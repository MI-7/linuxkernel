.intel_syntax noprefix
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

/*  0: Divide By Zero Exception */
_irq0:
    cli
    push 0
    push 0
    jmp irq_common_stub

/*  1: Debug Exception */
_isr1:
    cli
    push 0
    push 1
    jmp isr_common_stub

/*  2: Non Maskable Interrupt Exception */
_isr2:
    cli
    push 0
    push 2
    jmp isr_common_stub

/*  3: Int 3 Exception */
_isr3:
    cli
    push 0
    push 3
    jmp isr_common_stub

/*  4: INTO Exception */
_isr4:
    cli
    push 0
    push 4
    jmp isr_common_stub

/*  5: Out of Bounds Exception */
_isr5:
    cli
    push 0
    push 5
    jmp isr_common_stub

/*  6: Invalid Opcode Exception */
_isr6:
    cli
    push 0
    push 6
    jmp isr_common_stub

/*  7: Coprocessor Not Available Exception */
_isr7:
    cli
    push 0
    push 7
    jmp isr_common_stub

/*  8: Double Fault Exception (With Error Code!) */
_isr8:
    cli
    push 8
    jmp isr_common_stub

/*  9: Coprocessor Segment Overrun Exception */
_isr9:
    cli
    push 0
    push 9
    jmp isr_common_stub

/* 10: Bad TSS Exception (With Error Code!) */
_isr10:
    cli
    push 10
    jmp isr_common_stub

/* 11: Segment Not Present Exception (With Error Code!) */
_isr11:
    cli
    push 11
    jmp isr_common_stub

/* 12: Stack Fault Exception (With Error Code!) */
_isr12:
    cli
    push 12
    jmp isr_common_stub

/* 13: General Protection Fault Exception (With Error Code!) */
_isr13:
    cli
    push 13
    jmp isr_common_stub

/* 14: Page Fault Exception (With Error Code!) */
_isr14:
    cli
    push 14
    jmp isr_common_stub

/* 15: Reserved Exception */
_isr15:
    cli
    push 0
    push 15
    jmp isr_common_stub

/* 16: Floating Point Exception */
_isr16:
    cli
    push 0
    push 16
    jmp isr_common_stub

/* 17: Alignment Check Exception */
_isr17:
    cli
    push 0
    push 17
    jmp isr_common_stub

/* 18: Machine Check Exception */
_isr18:
    cli
    push 0
    push 18
    jmp isr_common_stub

/* 19: Reserved */
_isr19:
    cli
    push 0
    push 19
    jmp isr_common_stub

/* 20: Reserved */
_isr20:
    cli
    push 0
    push 20
    jmp isr_common_stub

/* 21: Reserved */
_isr21:
    cli
    push 0
    push 21
    jmp isr_common_stub

/* 22: Reserved */
_isr22:
    cli
    push 0
    push 22
    jmp isr_common_stub

/* 23: Reserved */
_isr23:
    cli
    push 0
    push 23
    jmp isr_common_stub

/* 24: Reserved */
_isr24:
    cli
    push 0
    push 24
    jmp isr_common_stub

/* 25: Reserved */
_isr25:
    cli
    push 0
    push 25
    jmp isr_common_stub

/* 26: Reserved */
_isr26:
    cli
    push 0
    push 26
    jmp isr_common_stub

/* 27: Reserved */
_isr27:
    cli
    push 0
    push 27
    jmp isr_common_stub

/* 28: Reserved */
_isr28:
    cli
    push 0
    push 28
    jmp isr_common_stub

/* 29: Reserved */
_isr29:
    cli
    push 0
    push 29
    jmp isr_common_stub

/* 30: Reserved */
_isr30:
    cli
    push 0
    push 30
    jmp isr_common_stub

/* 31: Reserved */
_isr31:
    cli
    push 0
    push 31
    jmp isr_common_stub


/* We call a C function in here. We need to let the assembler know
 that 'fault_handler' exists in another file */
.extern fault_handler

/* This is our common ISR stub. It saves the processor state, sets
 up for kernel mode segments, calls the C-level fault handler,
 and finally restores the stack frame. */
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, fault_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret
