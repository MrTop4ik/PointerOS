# PointerOS: A 32-bit x86 Hobby Kernel
A hobby operating system kernel written from scratch in **C** and **x86 Assembly**.

---

## Tech Stack
* **Language:** C (Freestanding, no standard library), x86 Assembly (NASM)
* **Architecture:** i686 (x86 32-bit)
* **Build System:** Makefile, Linker scripts
* **Emulation:** QEMU
* **Bootloader:** GRUB

## Current Features
Already Implemented:
- [x] **GDT & IDT:** Global and Interrupt Descriptor Tables configured.
- [x] **Interrupt Handling:** Hardware IRQs (PIC) and CPU Exceptions implemented.
- [x] **Paging:** Basic identity mapping and page directory setup.
- [x] **Drivers:** 
    - VGA
    - PIT
    - PS/2 Keyboard
- [x] **Paging:** Paging Enabled.
- [ ] **Physical Memory Manager:** Working on kmalloc and kfree implementation.

## Getting Started

### Prerequisites
You need a cross-compiler `i686-elf-gcc`, `nasm`, and `qemu`.

### Build & Run
```bash
# Clone the repository
git clone https://github.com/MrTop4ik/AreaOS/
cd AreaOS

# Compile and create ISO image
make iso

# Run in QEMU
make run
```