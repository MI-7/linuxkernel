bits 16
org 7E00h

jmp entry				; Jump to the entry point

%include "fat12.inc"
%include "stdio.inc"

entry:
	push cs				; Make the code segment = data segment
	pop ds

	mov ax, 0000h			; Set up the es register
	mov es, ax

	mov si, string_succes
	call printstring

	call a20			; Activates the A20 line
	call gdt			; Initialises the GDT table

	mov si, string_kernelname	; Name of the kernel file
	mov ax, 0050h			; Destination segment of the file
	mov bx, 0000h			; Destination offset of the file
	call loadfile			; Loads the kernel into memory

	call pmode			; Activates Protected Mode and jumps to the kernel

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialises A20					  ;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
a20:
	mov si, string_a20
	call printstring

	in al, 92h				; Store the byte
	or al, 2				; Set the second bit
	out 92h, al				; Reset the byte

	mov si, string_succes
	call printstring
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Sets up the GDT table					  ;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gdt:
	mov si, string_gdt
	call printstring

	cli					; Disable interrupts
	lgdt [gdt_pointer]			; Load the gdt table
	sti					; Restore interrupts

	mov si, string_succes
	call printstring

ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialises Protected Mode				  ;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
pmode:
	mov si, string_pmode
	call printstring

	cli 
	mov eax, cr0				; Fetch the cr0 bytes
	or ax, 1				; Set the first bit
	mov cr0, eax				; Restore the cr0 bytes

	jmp 08h:500h				; Location of the kernel entry point

	mov si, string_succes
	call printstring
ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Brings the CPU into an infinite loop			  ;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
hang:
	jmp hang

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; All data is stored here				  ;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

string_succes db 'Succes', 10, 13, 0
string_a20 db 'Initialising A20... ', 0
string_gdt db 'Initialising GDT... ', 0
string_pmode db 'Initialising Protected mode... ', 0
string_kernelname db 'KERNEL  TUK'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The GDT table entries					  ;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

gdt_table_start:
	dd 00000000h		; Null descriptor
	dd 00000000h

	dw 0xFFFF		; Limit
	dw 0000h		; Base
	db 00h			; Base
	db 10011010b		; Access byte - Code selector
	db 01001111b		; Flags
	db 00h			; Base

	dw 0xFFFF		; Limit
	dw 0000h		; Base
	db 00h			; Base
	db 10010010b		; Access byte - Data selector
	db 01001111b		; Flags
	db 00h			; Base

gdt_table_end:
gdt_pointer:
	dw gdt_table_end - gdt_table_start -1	; Size of gdt table
	dd gdt_table_start			; Pointer to the gdt table

