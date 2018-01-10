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

; symbolic constants
Esc_Key = 27

; current location = $ = current address
selfPtr DWORD $

; redefine value
COUNT = 10
COUNT = 20

; symbolic constants used in array def
array dword COUNT DUP(0)

; array size
array_size = ($ - array)

a_string BYTE "THIS IS A LONG STRING,..."
         BYTE " WITH A LOT OF CHARACTERS...", 0dh, 0ah, 0
a_string_len = ($ - a_string)

; array of word / double word
word_list WORD 1000h, 2000h, 3000h, 4000h
word_list_size = ($ - word_list) / 2
dword_list DWORD 1000h, 2000h, 3000h, 4000h
dword_list_size = ($ - dword_list) / 4

; name equ expression -> valid integer expr
; name equ symbol -> existing symbol
; name equ <text> -> something like a macro, simple replacement
matrix1 equ 10 * 10
matrix2 equ <10 * 10>
m1 word matrix1    ; m1 word 100
m2 word matrix2    ; m2 word 10 * 10

pressKey equ <"Press any key to continue...">
prompt byte pressKey
continueKey textequ <"Press again">
prompt1 byte continueKey


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