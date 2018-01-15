.intel_syntax noprefix

.section .data

.section .text
.globl _start

_start:
jmp end_program

end_program:
mov eax, 1
mov ebx, 2
int 0x80
