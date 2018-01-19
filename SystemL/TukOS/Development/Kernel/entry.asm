[bits 32]
[extern main]

entry:
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 7FFFFh

	call main

	jmp hang

hang:
	jmp hang

