CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mno-red-zone -mno-sse -mno-sse2 -mcmodel=kernel -Iinclude
AFLAGS = -f elf64
QEMUFLAGS = -d int -D logs/qemu.log -m 512 -serial stdio

iso:
	nasm 				$(AFLAGS) 		   arch/x86_64/boot/boot.s		-o boot.o
	nasm				$(AFLAGS)		   arch/x86_64/gdt/gdt.s		-o gdt.o
	nasm				$(AFLAGS)		   arch/x86_64/idt/idt.s		-o idt.o
	x86_64-elf-gcc 		$(CFLAGS)		-c kernel/kernel.c				-o kernel.o
	x86_64-elf-gcc		$(CFLAGS)		-c arch/x86_64/gdt/gdt.c		-o gdts.o
	x86_64-elf-gcc		$(CFLAGS)		-c arch/x86_64/idt/idt.c		-o idts.o
	x86_64-elf-gcc 		$(CFLAGS)		-c arch/x86_64/inlineasm.c		-o inlineasm.o
	x86_64-elf-gcc		$(CFLAGS)		-c arch/x86_64/pit.c			-o pit.o
	x86_64-elf-gcc		$(CFLAGS)		-c libc/string.c				-o string.o
	x86_64-elf-gcc		$(CFLAGS)		-c drivers/serial/serial.c		-o serial.o
	x86_64-elf-gcc		$(CFLAGS)		-c drivers/lfb/lfb.c			-o lfb.o
	x86_64-elf-gcc		$(CFLAGS)		-c mm/pmm.c 					-o pmm.o
	x86_64-elf-gcc		$(CFLAGS)		-c mm/vmm.c						-o vmm.o
	x86_64-elf-gcc		$(CFLAGS)		-c mm/buddy.c					-o buddy.o
	x86_64-elf-gcc		$(CFLAGS)		-c mm/slab.c					-o slab.o
	x86_64-elf-gcc		$(CFLAGS)		-c mm/kmalloc.c					-o kmalloc.o
	x86_64-elf-gcc -T arch/x86_64/boot/linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib -lgcc boot.o kernel.o inlineasm.o serial.o gdt.o gdts.o idt.o idts.o pit.o string.o pmm.o vmm.o lfb.o kmalloc.o buddy.o slab.o
	grub-file --is-x86-multiboot2 kernel.bin
	mv kernel.bin isodir/boot/kernel.bin
	grub-mkrescue -o kernel.iso isodir
	rm *.o

run: kernel.iso
	qemu-system-x86_64 kernel.iso $(QEMUFLAGS)