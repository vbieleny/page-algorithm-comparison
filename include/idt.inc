%macro idt_entry_error_code 3
extern %1
global %2
%2:
    pop eax
    pushad
    fxsave [%3]
    push eax
    call %1
    pop eax
    fxrstor [%3]
    popad
    iret
%endmacro

%macro idt_entry 3
extern %1
global %2
%2:
    pushad
    fxsave [%3]
    call %1
    fxrstor [%3]
    popad
    iret
%endmacro
