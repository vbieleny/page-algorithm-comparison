%macro idt_entry_error_code 3
extern %1
global %2
%2:
    call %1
    pop eax
    iretd
%endmacro

%macro idt_entry 3
extern %1
global %2
%2:
    call %1
    iretd
%endmacro
