;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This block describes the floppy format + Filesystem  	  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
