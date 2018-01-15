.intel_syntax noprefix

.section .data
var1:  .int 40
var2:  .int 20
var3:  .int 30

# other types:
var4:  .long 100
var5:  .byte 88
str1:  .ascii "hello"
str2:  .asciz "hello"

.section .text
.globl _start

_start:
mov eax, 5
add eax, 6
jmp _find_max

_find_max:
mov ecx, (var1)
cmp ecx, (var2)
jg check_3rd
mov ecx, (var2)

check_3rd:
cmp ecx, (var3)
jg end_program
mov ecx, (var3)

end_program:
mov eax, 1
mov ebx, 2
int 0x80
