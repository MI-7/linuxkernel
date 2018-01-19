BITS 16
ORG 0

%include "fat12.asm"		; Includes all the FAT12 filesystem functions

jmp entry			; Skip BIOS parameter block

OEMid			db '12345678'			; OS creator of disk
BytesPerSector 		dw 200h				; Bytes per sector
SectorsPerCluster	db 01h				; Sectors per cluster
ReservedSectors		dw 0001h			; Boot sector is reserved
FATs			db 02h				; 2 Fats are present
RootEntries		dw 00E0h			; Maximum of 224 root entries
SmallSectors		dw 0B40h			; 18*80*2 = 2880 sectors
MediaDescriptor		db 0xF0				; 3.5" floppy specifications
SectorsPerFAT		dd 0009h			; FAT covers 9 sectors
SectorsPerTrack		dw 0012h			; 18 Sectors per track
Heads			dw 0002h			; Number of heads of the floppy
HiddenSectors		dd 00000000h			; No hidden sectors present
LargeSectors		dd 00000000h			; We are using small sectors
PhysicalDriveNumber	db 00h				; BIOS takes care of this
Flags			db 00h				; Not used
Signature		db 29h				; Signature (no clue where it comes from)
ID			dd 00000000h			; Written at formatting
VolumeLabel		db 'VOLUME TUK'			; Volume name
SystemID		dd 'FAT12   '			; File System ID

entry:				; Entry point of bootloader

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Setting up the segments, stacks and Registers     	  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	cli
	mov ax, 07C0h		
	mov ds, ax			; Setup the data segment
	mov es, ax			; Setup the extra segment

	xor ax, ax
	mov ss, ax			; Zero the stack segment
	mov sp, 0xFFFF			; Stack pointer; Entry point of program
	sti

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Rest of the program flow			 	  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	call clearscreen		; Clears the screen

	mov si, stage2
	call printstring

	call loadstage2			; Loads stage 2

	call pause		

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parses the fat filesystem to load stage2 into memory    ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

loadstage2:

	xor cx, cx
	xor dx, dx

	mov ax, 0020h			; 32 bytes per directory entry
	mul WORD [RootEntries]		; Number of entries * 32
	div WORD [BytesPerSector]	; Number of sectors taken by the directory

	mov WORD [rootsize], ax		; Store the root size
	mov cx, ax			; Store rootsize into cx

	xor ax, ax
		
	mov al, BYTE [FATs]		; Number of FATs into al
	mul WORD [SectorsPerFAT]	; Number of FATs * sectors = Total sectors of FATs
	add ax, WORD [ReservedSectors]	; Add the first sectors to ax
	mov WORD [datasector], ax	; Store the number of sectors taken by FATs
	add WORD [datasector], cx	; Add the directory size to the size of the FATs
	mov WORD [rootsector], ax	; Sector the root starts

	mov ax, WORD [rootsector]	; Sector to start reading
	mov bx, 0200h			; Loading adress of the next sector to 1 sector after the bootloader
	mov cx, [rootsize]		; Number of sectors to load
	call loadsectors		; Load the sectors
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Root directory is loaded, so now the name is searched   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	mov cx, [RootEntries]		; If this reaches zero, the file isn't found
	mov di, 0200h			; Where we loaded the root directory
	mov si, stage2file
	

	comparename:
		push cx			; Remember number of entries
		mov cx, 11		; 11 characters per file name
		mov si, stage2file	; File name of the second stage into si
		push di			; Remember di position
		rep cmpsb		; Compare di character with si character 11 times
		pop di			; Restore previous di position
		je loadfat		; If the strings match, find the corresponding FATs
		pop cx			; Restore the number of root entries
		add di, 0020h		; Add 32 bytes to di to find the next directory entry
		loop comparename	; Decrease the number of entries remaining and search again

	loadfat:
		mov dx, WORD [di + 001Ah]	; Jumps to the 26th byte of the entry, which contains the first cluster
		mov WORD [filecluster], dx	; Stores the number of the first file cluster 

		xor ax, ax
		mov al, BYTE [FATs]		; 2 FATs present
		mul WORD [SectorsPerFAT]	; Find the sector size of the FATs
		mov cx, ax			; Store the number of sectors to load
		mov ax, WORD [ReservedSectors]	; FATs come after reserved sector, so this holds the first sector
		mov bx, 0200h			; Load FAT to a free memory space at 0:500
		call loadsectors		; Read the FATs into memory

		mov ax, 0050h
		mov es, ax
		mov bx, 0000h
		push bx				; Destination of file

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; FATs are loaded in 7C00h, so now we search for the first cluster   ;
; bx = Destination address of the file  			     ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		
	loadfile:
		mov ax, [filecluster]			; Put the cluster number into ax
		pop bx
		call convert_cluster_lba		; Store the LBA address into ax
		xor cx, cx				; Zero cx
		mov cl, BYTE [SectorsPerCluster]	; Number of sectors to read
		call loadsectors			; Load the cluster into the sector
		push bx

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Now we have to find the next cluster to load				  ;
; But first we have to test if we are dealing with an odd or even cluster ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
		mov ax, WORD [filecluster]		; Retrieve current cluster

		mov cx, ax				; Store cluster number in cx
		mov dx, ax				; Store cluster number in dx
		shr dx, 0001h				; Divide the cluster number by two
		add cx, dx				; Multiply the cluster by 1.5 to get the address (12bits an entry)
		mov bx, 0200h				; Location of the FAT in memory
		add bx, cx				; Beginning of FAT + 1.5(cluster) = Address of next cluster entry
		mov dx, WORD [bx]			; Read the two bytes in the FAT at the bx address
		test ax, 0001h				; Check if the cluster was even or odd
		jnz oddcluster				; If it is even, continue executing

	evencluster:
		and dx, 0000111111111111b		; Remove the first 4 bits of the cluster entry

		jmp nextcluster

	oddcluster:
		shr dx, 0004h				; Get rid of the last 4 bits

	nextcluster:
		mov WORD [filecluster], dx		; Store the new cluster
		cmp dx, 0FF8h				; Check if it is the last sector
		jb loadfile				; If not, load the next cluster
	
	lastcluster:

		push 0050h
		push 0000h
		retf

		call pause
	
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Loads sectors						  ;
; ax = First sector to read				  ;
; bx = First memory address to load sector to		  ;
; cx = Number of sectors to read			  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

loadsectors:	
	push ax						; Remember the sector to read
	push bx						; Remember the address to load to
	push cx						; Remember the number of sectors to read
			
	call convert_lba_chs			; Convert the ax register to the correct CHS

	mov ah, 02h				; Read sector function
	mov al, 01h				; Read a single sector
	mov ch, BYTE [floppytrack]		; Track to read
	mov cl, BYTE [floppysector]		; Sector to read
	mov dh, BYTE [floppyhead]		; Head to read
	mov dl, BYTE [PhysicalDriveNumber]	; Drive to read	
	int 13h

	jnc success				; Jump to success when no error flag was set

	xor ax, ax				; Reset function
	int 13h					; Reset floppy drive

	pop cx					; Recover registers
	pop bx
	pop ax

	call loadsectors			; Retry loading sectors
			
	success:				; Successfully read a sector
		pop cx				; Recover registers
		pop bx
		pop ax

		add bx, 0200h			; Load to a next sector in memory
		inc ax				; Increase the sector to load by one	
	
		loop loadsectors		; Keep reading untill cx = 0	
	ret					; Finished loading sectors

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Converts a linear sector address to a CHS location      ;
; ax = LBA						  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

convert_lba_chs:
	xor dx, dx				; Zero the dx 
	div WORD [SectorsPerTrack]		; Find the track number by dividing by ax
	inc dl					; increase the remainder by one			
	mov BYTE [floppysector], dl		; Store the floppy sector
		
	xor dx, dx
				
	div WORD [Heads]			; Divide ax (number of tracks) by track 
	mov BYTE [floppyhead], dl
	mov BYTE [floppytrack], al
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Converts a cluster to a linear address		  ;
; 	ax = cluster number				  ;
; output:						  ;
; 	ax = LBA address				  ;						  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

convert_cluster_lba:	; Function assumes 1 sector per cluster only
	sub ax, 0002h	; Subtract 2 from the cluster number
	xor cx, cx	
	mov cl, BYTE [SectorsPerCluster]
	mul cx
	add ax, WORD [datasector]
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Clears the screen					  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

clearscreen:
	push ax
	push bx
	push cx
	push dx

	mov ah, 06h		; Clear screen function
	xor al, al		; Number of lines (0 = clear screen)
	mov bh, 07h		; White on black attribute
	xor cx, cx		; Begin at top left
	mov dx, 184Fh		; Total number of lines
	int 10h			; Clear screen int

	mov ah, 02h		; Cursor position function
	mov bh, 00h		; Page number
	xor dx, dx		; Top left corner
	int 10h			; Reset cursor int

	pop ax
	pop bx
	pop cx
	pop dx
ret

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
		lodsb			; Put char si into ah and si++
		or al, al		; Check if end of string is reached
		jz end			; End of string is reached

		int 10h			; Interupt to display char
		jmp loadchar		; Print new char

	end:				; End of string is reached
		pop bx
		pop ax
ret					; Return to start

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

	pop bx
	pop ax
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Brings the CPU into an infinite loop			  ;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
pause:
	jmp pause

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; All data is stored here				  ;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

datasector dw 00h		; Sector at which the data starts
floppysector db 00h		; Floppy sector to ready
floppyhead db 00h		; Floppy head to ready
floppytrack db 00h		; Floppy track to ready
rootsize dw 0000h		; Size of directory root
rootsector dw 0000h		; Starting sector of directory root
filecluster dw 0000h		; Contains the first file cluster for the FAT

stage2file db 'STAGE2  TUK'	; File name of the second stage

stage2 db 'Loading Stage 2... ', 0

times 510-($-$$) db 0		
dw 0xAA55
