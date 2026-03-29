CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
NFLAGS = -f elf
QEMUFLAGS = -d int -D logs/qemu.log

iso:
	nasm 			$(NFLAGS)          src/boot.s					-o boot.o
	nasm 			$(NFLAGS)		   src/idt/idt.s      			-o idt.o
	nasm			$(NFLAGS)		   src/gdt/gdt.s				-o gdt.o
	i686-elf-gcc	$(CFLAGS)		-c src/memory.c					-o memory.o
	i686-elf-gcc    $(CFLAGS) 		-c src/idt/idt.c 	   			-o idts.o
	i686-elf-gcc    $(CFLAGS) 		-c src/pit.c 					-o pit.o
	i686-elf-gcc    $(CFLAGS) 		-c src/keyboard.c				-o keyboard.o
	i686-elf-gcc    $(CFLAGS) 		-c src/utils/utils.c			-o utils.o
	i686-elf-gcc    $(CFLAGS) 		-c src/utils/inlineasm.c		-o inlineasm.o
	i686-elf-gcc    $(CFLAGS) 		-c src/gdt/gdt.c 				-o gdts.o
	i686-elf-gcc    $(CFLAGS) 		-c src/vga/vga.c 				-o vga.o
	i686-elf-gcc    $(CFLAGS) 		-c src/kernel.c 				-o kernel.o
	i686-elf-gcc -T linker.ld -o kernel -ffreestanding -O2 -nostdlib boot.o kernel.o vga.o gdt.o gdts.o utils.o idt.o idts.o inlineasm.o memory.o pit.o keyboard.o -lgcc
	grub-file --is-x86-multiboot kernel
	mv kernel isodir/boot/kernel
	grub-mkrescue -o kernel.iso isodir/
	rm *.o

run: kernel.iso
	qemu-system-i386 kernel.iso $(QEMUFLAGS)

rmo:
	rm *.o
