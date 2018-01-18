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

_irq0:
    cli
    push 0
    push 32
    jmp irq_common_stub

_irq1:
    cli
    push 0
    push 33
    jmp irq_common_stub

/*  2: Non Maskable Interrupt Exception */
_irq2:
    cli
    push 0
    push 34
    jmp irq_common_stub

/*  3: Int 3 Exception */
_irq3:
    cli
    push 0
    push 35
    jmp irq_common_stub

/*  4: INTO Exception */
_irq4:
    cli
    push 0
    push 36
    jmp irq_common_stub

/*  5: Out of Bounds Exception */
_irq5:
    cli
    push 0
    push 37
    jmp irq_common_stub

/*  6: Invalid Opcode Exception */
_irq6:
    cli
    push 0
    push 38
    jmp irq_common_stub

/*  7: Coprocessor Not Available Exception */
_irq7:
    cli
    push 0
    push 39
    jmp irq_common_stub

/*  8: Double Fault Exception (With Error Code!) */
_irq8:
    cli
    push 0
    push 40
    jmp irq_common_stub

/*  9: Coprocessor Segment Overrun Exception */
_irq9:
    cli
    push 0
    push 41
    jmp irq_common_stub

/* 10: Bad TSS Exception (With Error Code!) */
_irq10:
    cli
    push 0
    push 42
    jmp irq_common_stub

/* 11: Segment Not Present Exception (With Error Code!) */
_irq11:
    cli
    push 0
    push 43
    jmp irq_common_stub

/* 12: Stack Fault Exception (With Error Code!) */
_irq12:
    cli
    push 0
    push 44
    jmp irq_common_stub

/* 13: General Protection Fault Exception (With Error Code!) */
_irq13:
    cli
    push 0
    push 45
    jmp irq_common_stub

/* 14: Page Fault Exception (With Error Code!) */
_irq14:
    cli
    push 0
    push 46
    jmp irq_common_stub

/* 15: Reserved Exception */
_irq15:
    cli
    push 0
    push 47
    jmp irq_common_stub


/* We call a C function in here. We need to let the assembler know
 that 'fault_handler' exists in another file */
.extern irq_handler

/* This is our common ISR stub. It saves the processor state, sets
 up for kernel mode segments, calls the C-level fault handler,
 and finally restores the stack frame. */
irq_common_stub:
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
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret
