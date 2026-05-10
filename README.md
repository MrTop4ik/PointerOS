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
- [x] **KMemory Allocation:**
    - **PMM** Implemented.
    - **VMM** Implemented.
    - **KMAlloc:**
        - **Buddy Allocator** Implemented.
        - **Slab Allocator** Implemented.
- [ ] **Drivers:** 
    - **LFB** Implemented.
    - **QEMU Serial** Implemented.

## Getting Started

### Prerequisites
| Category | Tool | Notes |
| :--- | :--- | :--- |
| **Cross-Compiler** | `x86_64-elf-gcc` | Required `-mno-red-zone` flag to prevent stack corruption |
| **Assembler** | `nasm` | Used for bootloader entry and interrupt stubs |
| **Emulator** | `QEMU` | Run with `-machine q35` and `-bios OVMF.fd` |
| **ISO Creation** | `grub-common` | Uses `grub-mkrescue` to build the bootable image |
| **UEFI Support** | `grub-efi-amd64-bin` | Provides modules for `UEFI` booting and `ACPI 2.0` |

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