; AddTwo.asm - adds two 32-bit integers.
; Chapter 3 example

.386
.model flat,stdcall
.stack 4096
ExitProcess proto,dwExitCode:dword

.data
sum DWORD 0
var1 DWORD 100
var2 DWORD 200
var3 DWORD 50

.code
add_two proc
	mov	eax,5
	add	eax,6
	mov sum,eax

	invoke ExitProcess,0    ; add numbers and exit
add_two endp

compare_three proc
    mov ecx, var1
	cmp ecx, var2
	jg check_3rd
	mov ecx, var2
check_3rd:
	cmp ecx, var3
	jg end_compare_three
	mov ecx, var3
end_compare_three:
	jmp add_two     ; jump to exit point, add_two is an address
compare_three endp

end compare_three       ; specifies entry point