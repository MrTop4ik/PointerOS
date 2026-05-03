CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mno-red-zone -mno-sse -mno-sse2 -mcmodel=kernel
AFLAGS = -f elf64
QEMUFLAGS = -d int -D logs/qemu.log -m 512 -serial stdio

iso:
	nasm 				$(AFLAGS) 		   src/boot.s					-o boot.o
	nasm				$(AFLAGS)		   src/gdt/gdt.s				-o gdt.o
	nasm				$(AFLAGS)		   src/idt/idt.s				-o idt.o
	x86_64-elf-gcc 		$(CFLAGS)		-c src/kernel.c					-o kernel.o
	x86_64-elf-gcc 		$(CFLAGS)		-c src/utils/inlineasm.c		-o inlineasm.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/utils/string.c			-o string.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/serial/serial.c			-o serial.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/gdt/gdt.c				-o gdts.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/idt/idt.c				-o idts.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/pit.c					-o pit.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/memory/pmm.c 			-o pmm.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/memory/vmm.c				-o vmm.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/memory/buddy.c			-o buddy.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/memory/slab.c			-o slab.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/memory/kmalloc.c			-o kmalloc.o
	x86_64-elf-gcc		$(CFLAGS)		-c src/lfb/lfb.c				-o lfb.o
	x86_64-elf-gcc -T linker.ld -o kernel -ffreestanding -O2 -nostdlib -lgcc boot.o kernel.o inlineasm.o serial.o gdt.o gdts.o idt.o idts.o pit.o string.o pmm.o vmm.o lfb.o kmalloc.o buddy.o slab.o
	grub-file --is-x86-multiboot2 kernel
	mv kernel isodir/boot/kernel
	grub-mkrescue -o kernel.iso isodir
	rm *.o

run: kernel.iso
	qemu-system-x86_64 kernel.iso $(QEMUFLAGS)