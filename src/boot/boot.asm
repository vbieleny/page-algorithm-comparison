bits 16                     ; This is implicit but we declare to make it clear that we are using 16-bit instructions
org 0x7c00                  ; Our code will be loaded at 0x7C00 by BIOS, so we tell NASM we want to offset all addreses by 0x7C00

xchg bx, bx                 ; Debugging breakpoint for Bochs debugger on bootloader start
jmp 0x0000:_main            ; Far jump to ensure that CS = 0x0000

%include "helpers.inc"
%include "gdt.inc"

load_kernel_chunk:
    pushad                                  ; Store all general purpose registers to stack
    
    mov ah, 0x42                            ; Extended Read Sectors From Drive
    mov dl, [drive_index]                   ; Drive index (e.g. 1st HDD = 0x80)
    mov si, dap                             ; Segment:Offset pointer to the DAP (Disk Address Packet)
    int 0x13                                ; Read sectors from boot drive

    mov ecx, LOAD_BYTES
    mov esi, 0x7e00
    mov edi, [load_address]
    rep a32 movsb                           ; Copy loaded sectors from temporary buffer to kernel target address

    add dword [dap.start], LOAD_SIZE        ; Increase from what sector we will be reading next
    add dword [load_address], LOAD_BYTES    ; Increase where we will copy next sectors

    popad                                   ; Restore all general purpose registers from stack
    ret

; Bootloader entry point
_main:
    ; Initialize essential segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov [drive_index], dl   ; Store boot drive index (BIOS loads boot drive index into DL register)

    mov sp, 0x7c00          ; Set stack pointer to beginning of bootloader growing downwards

    cld                     ; Clear the direction flag (data goes onwards for string operations)

    call clear_screen

    mov ax, 0x2401          ; Activate A20 gate
    int 0x15
    
    cli                     ; Disable interrupts when loading GDT since interrupt could happen when CPU is in inconsistent state
    push ds                 ; Push data segemnt to stack for Unreal mode
    push es                 ; Push extra segment to stack for Unreal mode
    lgdt [gdtr]             ; Load our Global Descriptor Table

    mov eax, cr0
    or al, 1
    mov cr0, eax            ; Set PE (Protected Mode Enable) bit in control register CR0 to enable protected mode

    mov bx, 0x10            ; Load our data segment descriptor as defined in GDT
    mov ds, bx              ; Load segment descriptor cache register with so we can use 32-bit offsets in real mode later
    mov es, bx              ; Load segment descriptor cache register with so we can use 32-bit offsets in real mode later

    and al, 0xfe
    mov cr0, eax            ; Clear PE (Protected Mode Enable) bit to get to unreal mode (we can do it because we are in 16-bit protected mode)

    pop es                  ; Restore extra segment to previous value
    pop ds                  ; Restore data segment to previous value
    sti

    mov cx, KERNEL_SIZE / LOAD_SIZE
.load_loop:
    call load_kernel_chunk  ; Load LOAD_SIZE sectors into temporary kernel buffer and copy it to final kernel destination address
    loop .load_loop         ; Loop until whole kernel is loaded

    cli
    mov eax, cr0
    or al, 1
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

    xchg bx, bx                 ; Debugging breakpoint for Bochs debugger before jumping to kernel
    call 0x08:KERNEL_ADDRESS    ; Jump to C kernel code (at address 0x100000)
.hang:
    hlt
    jmp .hang                   ; Endless jump to current address to halt CPU (should never get here)

bits 16                         ; Switch back to 16-bit mode (not necessary - just in case if there will be some code)

KERNEL_SIZE equ 256             ; Kernel size in sectors
KERNEL_ADDRESS equ 0x100000     ; Where will our kernel reside

LOAD_SIZE equ 64                ; How many sectors to load with one INT13 call
LOAD_BYTES equ LOAD_SIZE * 512  ; How many bytes will be loaded with one INT13 call

drive_index db 0                ; Index of boot drive that this bootloader was loaded from
load_address dd KERNEL_ADDRESS  ; Address where next chunk of kernel will be copied

dap:                                ; Disk Address Packet
        db 0x10                     ; Size of packet (0x10 or 0x18)
        db 0x00                     ; Reserved (0)
        dw LOAD_SIZE                ; Number of blocks to transfer (max 0x007F for Phoenix EDD)
        dw 0x7e00                   ; Destination offset
        dw 0x0000                   ; Destination segment
.start  dd 0x0001                   ; Starting absolute block number
        dd 0x0000                   ; Starting absolute block number

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
