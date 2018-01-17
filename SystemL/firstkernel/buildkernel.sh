rm boot.o kernel.o gdt_asm.o gdt.o utility.o test.o terminal.o
i686-elf-as boot.s -o boot.o
i686-elf-as gdt.s -o gdt_asm.o
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c utility.c -o utility.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c test.c -o test.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
echo "linking..."
i686-elf-gcc -T linker.ld -o systeml.bin -ffreestanding -O2 -nostdlib boot.o kernel.o gdt_asm.o gdt.o utility.o terminal.o test.o -lgcc
./testmultiboot.sh
cp systeml.bin ./iso/boot/systeml.bin
