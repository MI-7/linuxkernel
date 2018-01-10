; Template File

.386
.model flat,stdcall
.stack 4096
ExitProcess proto,dwExitCode:dword

.data
; declare your variables here

.code
your_proc proc
    ; your code here
	invoke ExitProcess,0
your_proc endp
end your_proc