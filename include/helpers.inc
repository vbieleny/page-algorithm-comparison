; Clears the screen by setting the default VGA text mode
clear_screen:
    mov ah, 0x00            ; Set video mode
    mov al, 0x03            ; Text mode 80x25 16 colors
    int 0x10
    ret

; Print a character on screen as a teletype output
; Inputs:
; AL - ASCII character to print
print_char:
    push bx
    mov ah, 0x0e            ; Teletype output
    xor bx, bx              ; BH = Page Number, BL = Color (only in graphic mode)
    int 0x10                ; Print character that is in AL register
    pop bx
    ret

; Prints a new line (as CRLF) on teletype output
print_new_line:
    mov al, 13              ; Carriage return
    call print_char
    mov al, 10              ; Line feed
    call print_char
    ret

print_al:
    push dx
    push ax
    push bx
    mov dx, ax
    
    mov bx, dx
    xor bh, bh
    shr bl, 4
    and bl, 0x0f
    mov al, [hex_table + bx]
    call print_char

    mov bx, dx
    xor bh, bh
    and bl, 0x0f
    mov al, [hex_table + bx]
    call print_char

    pop bx
    pop ax
    pop dx
    ret

; Prints null-terminated string to teletype output with BIOS interrupt 0x10
; Inputs:
; DS:SI - ASCII string to print (must end with 0)
print_string:
    mov ah, 0x0e            ; Teletype output
    xor bx, bx              ; BH = Page Number, BL = Color (only in graphic mode)
.L1:
    lodsb                   ; Load character we want to print from DS:SI to AL and increment DS:SI
    int 0x10
    
    cmp byte [si], 0        ; Check if we are on null character (0)
    jne .L1                 ; If we are not, print next character
    ret

hex_table: db "0123456789abcdef"
