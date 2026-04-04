CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mno-red-zone
AFLAGS = -f elf64
QEMUFLAGS = -d int -D logs/qemu.log -serial stdio

iso:
	nasm 				$(AFLAGS) 		   src/boot.s					-o boot.o
	x86_64-elf-gcc 		$(CFLAGS)		-c src/kernel.c					-o kernel.o
	x86_64-elf-gcc 		$(CFLAGS)		-c src/utils/inlineasm.c		-o inlineasm.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/serial/serial.c			-o serial.o
	x86_64-elf-gcc -T linker.ld -o kernel -ffreestanding -O2 -nostdlib -lgcc boot.o kernel.o inlineasm.o serial.o
	grub-file --is-x86-multiboot2 kernel
	mv kernel isodir/boot/kernel
	grub-mkrescue -o kernel.iso isodir
	rm *.o

run: kernel.iso
	qemu-system-x86_64 kernel.iso $(QEMUFLAGS)