; Macro that defines a GDT entry
%macro gdt_entry 7
%1:                         ; GDT entry label
    dw %2                   ; Segment limit first 0-15 bits
    dw %3                   ; Base first 0-15 bits
    db %4                   ; Base 16-23 bits
    db %5                   ; Access byte
    db %6                   ; High 4 bits (flags), low 4 bits (limit 4 last bits) (limit is 20 bit wide)
    db %7                   ; Base 24-31 bits
%endmacro
