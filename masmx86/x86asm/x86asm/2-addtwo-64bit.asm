; addtwo asm - 64 bit version

ExitProcess proto

.data
sum DWORD 0

.code
add_two_64 proc
    mov eax, 5
	add eax, 6
	mov sum, eax

	mov ecx, 0
	call ExitProcess
add_two_64 endp
end