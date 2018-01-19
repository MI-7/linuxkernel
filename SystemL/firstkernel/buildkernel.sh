rm boot.o kernel.o gdt_asm.o gdt.o utility.o test.o terminal.o io.o interrupt.o idt_asm.o idt.o irq_asm.o
i686-elf-as -g boot.s -o boot.o
i686-elf-as -g gdt.s -o gdt_asm.o
i686-elf-as -g idt.s -o idt_asm.o
i686-elf-as -g irq.s -o irq_asm.o
i686-elf-gcc -g -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -g -m32 -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -g -m32 -c utility.c -o utility.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -g -m32 -c terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -g -m32 -c io.c -o io.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -g -m32 -c interrupt.c -o interrupt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -g -m32 -c test.c -o test.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -g -m32 -c idt.c -o idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
echo "linking..."
i686-elf-gcc -g -m32 -T linker.ld -o systeml.bin -ffreestanding -O2 -nostdlib boot.o kernel.o gdt_asm.o gdt.o utility.o terminal.o test.o io.o interrupt.o idt_asm.o idt.o irq_asm.o -lgcc
./testmultiboot.sh
cp systeml.bin ./iso/boot/systeml.bin
