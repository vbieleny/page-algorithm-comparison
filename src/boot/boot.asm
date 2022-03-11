bits 16                     ; This is implicit but we declare to make it clear that we are using 16-bit instructions
org 0x7c00                  ; Our code will be loaded at 0x7C00 by BIOS, so we tell NASM we want to offset all addreses by 0x7C00

xchg bx, bx                 ; Debugging breakpoint for Bochs debugger on bootloader start
jmp 0x0000:_main            ; Far jump to ensure that CS = 0x0000

%include "helpers.inc"
%include "gdt.inc"

; Bootloader entry point
_main:
    ; Initialize essential segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov sp, 0x7c00          ; Set stack pointer to beginning of bootloader growing downwards

    cld                     ; Clear the direction flag (data goes onwards for string operations)

    call clear_screen

    mov ax, 0x2401          ; Activate A20 gate
    int 0x15
    
    cli                     ; Disable interrupts when loading GDT since interrupt could happen when CPU is in inconsistent state
    lgdt [gdtr]             ; Load our Global Descriptor Table

    mov eax, cr0
    or eax, 1
    mov cr0, eax            ; Set PE (Protected Mode Enable) bit in control register CR0 to enable protected mode

    jmp 0x08:protected_mode ; Far jump to clear prefetch queue and set selector to segment descriptor for code

bits 32                     ; Switch to 32-bit instructions as we are already in 32-bit protected mode
protected_mode:
    mov eax, 0x10           ; Selector for data descriptor

    ; Set all segment registers (except code segment) to data descriptor
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax

    mov esp, 0xf00000           ; Set stack pointer at 16MB (0xf00000) so it won't be indentity mapped by paging

    mov eax, 1			        ; Starting sector we want to load from (0 is boot sector)
	mov ecx, KERNEL_SIZE	    ; Total number of sectors we want to load
	mov edi, KERNEL_ADDRESS     ; Address we want to load these sectors in to (1 MB)
	call ata_lba_read           ; Read kernel from ATA device

    xchg bx, bx                 ; Debugging breakpoint for Bochs debugger before jumping to kernel
    call 0x08:KERNEL_ADDRESS    ; Jump to C kernel code (at address 0x100000)
.hang:
    hlt
    jmp .hang                   ; Endless jump to current address to halt CPU (should never get here)

%include "ata.inc"

bits 16                         ; Switch back to 16-bit mode (not necessary - just in case if there will be some code)

KERNEL_SIZE equ 255             ; Kernel size in sectors
KERNEL_ADDRESS equ 0x100000     ; Where will our kernel reside

align 16                                                                ; Align GDT as some CPUs like them aligned
gdtr:
    dw gdt_end - gdt_start - 1                                          ; Size of GDT table
    dd gdt_start                                                        ; Offset of GDT table
gdt_start:
    dq 0                                                                ; Required null descriptor
gdt_entry gdt_code, 0xffff, 0x0000, 0x00, 0b10011010, 0b11001111, 0x00  ; 32-bit code GDT entry
gdt_entry gdt_data, 0xffff, 0x0000, 0x00, 0b10010010, 0b11001111, 0x00  ; 32-bit data GDT entry
gdt_end:

times 510-($-$$) db 0       ; Fill remaining space with zeroes so that bootloader has 512 bytes
dw 0xaa55                   ; Magic boot sequence for BIOS at the end of bootloader
