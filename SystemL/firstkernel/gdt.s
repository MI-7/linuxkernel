.intel_syntax noprefix

.global _gdt_flush
.extern gdt_ptr

_gdt_flush:
	lgdt [gdt_ptr]
	jmp 0x08:reload_CS
reload_CS:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret
