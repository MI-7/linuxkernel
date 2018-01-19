
%include "bpb.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parses the fat filesystem to load a file into memory    ;
; Input:						  ;
; 	si: 11 byte file name string			  ;
;	ax: Segment to read to				  ;
;	bx: Offset to read to				  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

loadfile:
	mov WORD [destinationsegment], ax	; Store the segment destination of the file
	mov WORD [destinationoffset], bx	; Store the offset destination of the file
		
	mov cx, 11d			; Loop 11 times (11 byte file name)
	mov di, filename		; Store the filename into the destination index

	savename:	
		movsb			; Copy si to di and increment both by one
		loop savename		; Repeat 11 times

loaddirectory:
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
	mov bx, 4200h			; Loading adress of the next sector to 1 sector after the bootloader
	mov cx, [rootsize]		; Number of sectors to load
	call loadsectors		; Load the sectors
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Root directory is loaded, so now the name is searched   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	mov cx, [RootEntries]		; If this reaches zero, the file isn't found
	mov di, 4200h			; Where we loaded the root directory
	mov si, filename
	

	findname:
		push cx			; Remember number of entries
		mov cx, 11		; 11 characters per file name
		mov si, filename	; File name of the second stage into si
		push di			; Remember di position
		rep cmpsb		; Compare di character with si character 11 times
		pop di			; Restore previous di position
		je loadfat		; If the strings match, find the corresponding FATs
		pop cx			; Restore the number of root entries
		add di, 0020h		; Add 32 bytes to di to find the next directory entry
		loop findname		; Decrease the number of entries remaining and search again
		hlt

	loadfat:
		mov dx, WORD [di + 001Ah]	; Jumps to the 26th byte of the entry, which contains the first cluster
		mov WORD [filecluster], dx	; Stores the number of the first file cluster 

		xor ax, ax
		mov al, BYTE [FATs]		; 2 FATs present
		mul WORD [SectorsPerFAT]	; Find the sector size of the FATs
		mov cx, ax			; Store the number of sectors to load
		mov ax, WORD [ReservedSectors]	; FATs come after reserved sector, so this holds the first sector
		mov bx, 4200h			; Load FAT to a free memory space at 
		call loadsectors		; Read the FATs into memory

		mov ax, WORD [destinationsegment]
		mov es, ax
		mov bx, WORD [destinationoffset]
		push bx				; Destination of file

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; FATs are loaded in 7C00h, so now we search for the first cluster   ;return ascii
; bx = Destination address of the file  			     ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		
	loaddata:
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
		mov bx, 4200h				; Location of the FAT in memory
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
		jb loaddata				; If not, load the next cluster
	
	lastcluster:
		add sp, 4				; Remove the top 2 entries from the stack for a proper ret instruction
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

convert_cluster_lba:			; Function assumes 1 sector per cluster only
	sub ax, 0002h			; Subtract 2 from the cluster number
	xor cx, cx	
	mov cl, BYTE [SectorsPerCluster]
	mul cx
	add ax, WORD [datasector]
ret

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

destinationsegment dw 0000h	; Store the destination segment
destinationoffset dw 0000h	; Store the destination offset
filename db '           ', 0	; File name string

