;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Prints a string					  ;	
; si = string						  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

printstring:
	push ax
	push bx
			
	mov ah, 0Eh			; Print char function
	mov bh, 00h			; Page number
	
	loadchar:
		lodsb			; Put char si into al and si++
		or al, al		; Check if end of string is reached
		jz end			; End of string is reached

		int 10h			; Interupt to display char
		jmp loadchar		; Print new char

	end:				; End of string is reached
		pop bx
		pop ax
ret	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Prints a single byte					  ;	
; al = character					  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

printchar:
	push ax
	push bx

	mov ah, 0Eh		; Print char function
	mov bh, 00h		; Page number
	int 10h			; Interupt for printing a character

	mov al, 10
	int 10h

	mov al, 13
	int 10h

	pop bx
	pop ax
ret
