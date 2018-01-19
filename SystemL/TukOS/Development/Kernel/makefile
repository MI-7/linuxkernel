kernel.tuk: entry.asm linker.ld main.c main.h video.c video.h common.c common.h memory.c memory.h interrupts.asm interrupts.c interrupts.h keyboard.c keyboard.h stdio.c stdio.h shell.c shell.h
	nasm -f elf entry.asm -o entry.o
	nasm -f elf interrupts.asm -o interruptsasm.o
	gcc -c main.c -o main.o -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -nostdinc
	gcc -c video.c -o video.o -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -nostdinc
	gcc -c common.c -o common.o -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -nostdinc
	gcc -c maths.c -o maths.o -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -nostdinc
	gcc -c memory.c -o memory.o -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -nostdinc
	gcc -c interrupts.c -o interrupts.o -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -nostdinc
	gcc -c keyboard.c -o keyboard.o -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -nostdinc
	gcc -c stdio.c -o stdio.o -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -nostdinc
	gcc -c shell.c -o shell.o -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -nostdinc
	ld -T linker.ld -o kernel.tuk entry.o main.o video.o common.o maths.o memory.o interruptsasm.o interrupts.o keyboard.o stdio.o shell.o


