# PointerOS: A 64-bit x86 Hobby Kernel

A hobby operating system kernel written from scratch in **C** and **x86 Assembly**.

---

## Tech Stack
* **Language:** C (Freestanding, no standard library), x86 Assembly (NASM)
* **Architecture:** x86_64 (x86 64-bit)
* **Build System:** Makefile, Linker scripts
* **Emulation:** QEMU
* **Bootloader:** GRUB (Multiboot2)

## Current Features
Already Implemented:
- [x] **GDT:** Global Descriptor Table configured.
- [x] **IDT** Interrupt Descriptor Table configured.
- [x] **Paging:** 64-bit Paging Enabled.
- [x] **Higher Half Kernel:** Implemeted.
- [ ] **Memory Allocation:**
    - **PMM** Implemented.
    - **VMM** In Progress.
- [ ] **Drivers:** 
    - **LFB** In Progress. 
    - **PIT** Implemented.

## Getting Started

### Prerequisites
You need a cross-compiler `x86_64-elf-gcc` (with no red-zone), `nasm`, and `qemu`.

### Build & Run
```bash
# Clone the repository
git clone https://github.com/MrTop4ik/PointerOS/
cd PointerOS

# Compile and create ISO image
make iso

# Run in QEMU
make run
```