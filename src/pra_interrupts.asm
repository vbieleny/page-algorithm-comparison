bits 32

%include "pra_idt.inc"

section .text

idt_entry_error_code pra_page_fault_handler, asm_page_fault_handler, fx_memory_page_fault
idt_entry_error_code pra_kernel_panic_handler, asm_kernel_panic_handler, fx_memory_kernel_panic
idt_entry pra_timer_interrupt, asm_timer_interrupt, fx_memory_timer_interrupt

section .data
align 16
fx_memory_page_fault: times 512 db 0
fx_memory_kernel_panic: times 512 db 0
fx_memory_timer_interrupt: times 512 db 0
