
[global int_general]
[global int_0]
[global int_1]
[global int_2]
[global int_3]
[global int_4]
[global int_5]
[global int_6]
[global int_7]
[global int_8]
[global int_9]
[global int_10]
[global int_11]
[global int_12]
[global int_13]
[global int_14]
[global int_15]
[global int_16]
[global int_17]
[global int_18]
[global int_19]
[global int_20]
[global int_21]
[global int_22]
[global int_23]
[global int_24]
[global int_25]
[global int_26]
[global int_27]
[global int_28]
[global int_29]
[global int_30]
[global int_31]
[global int_32]
[global int_33]


[extern isr_general]
[extern isr_33]

int_general:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_0:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret
	
int_1:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_2:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_3:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_4:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_5:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_6:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_7:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_8:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_9:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_10:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_11:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_12:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_13:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_14:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_15:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_16:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_17:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_18:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_19:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_20:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_21:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_22:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_23:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_24:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_25:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_26:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_27:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_28:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_29:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_30:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_31:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_32:
pusha
push ds
push es
push fs
push gs

	call isr_general

pop gs
pop fs
pop es
pop ds
popa
iret

int_33:
pusha
push ds
push es
push fs
push gs

	call isr_33

pop gs
pop fs
pop es
pop ds
popa
iret
