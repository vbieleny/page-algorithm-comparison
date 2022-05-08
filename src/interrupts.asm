bits 32

%include "idt.inc"

section .text

idt_entry_error_code pra_page_fault_handler, asm_page_fault_handler, fx_memory_page_fault
idt_entry_error_code pra_kernel_panic_handler, asm_kernel_panic_handler, fx_memory_kernel_panic
idt_entry pra_timer_interrupt, asm_timer_interrupt, fx_memory_timer_interrupt

section .data
align 16
