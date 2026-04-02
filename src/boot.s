MB2_MAGIC       equ   0xE85250D6
MB2_ARCH        equ   0
MB2_HEADER_LEN  equ   header_end - header_start
MB2_MAGIC_SUM   equ   -(MB2_MAGIC + MB2_ARCH + MB2_HEADER_LEN)

section .multiboot_header
align 8
header_start:
    dd MB2_MAGIC
    dd MB2_ARCH
    dd MB2_HEADER_LEN
    dd MB2_MAGIC_SUM

    dw 0
    dw 0
    dd 8
header_end:

section .bss

align 4096
pml4: resb 4096
pdpt: resb 4096
pd: resb 4096
stack_top:
    resb 32768
stack_bottom:

section .rodata

gdt64:
    dq 0
.code_segment: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.data_segment: equ $ - gdt64 
    dq (1 << 41) | (1 << 44) | (1 << 47)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

section .text
bits 32
global _start
_start:
    mov esp, stack_top

    mov edi, eax
    mov esi, ebx

    mov edi, pml4
    xor eax, eax
    mov ecx, 4096 * 3 / 4
    rep stosd

    mov eax, pdpt
    or eax, 0b11
    mov [pml4], eax

    mov eax, pd
    or eax, 0b11
    mov [pdpt], eax

    mov eax, 0b10000011
    mov [pd], eax
    
    mov eax, pml4
    mov cr3, eax
    
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [gdt64.pointer]

    jmp gdt64.code_segment:long_mode_entry

bits 64
long_mode_entry:
    mov ax, gdt64.data_segment
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rsp, stack_top

    extern kernel_main
    call kernel_main
     
    hlt