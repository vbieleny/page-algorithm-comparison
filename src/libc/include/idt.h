#pragma once

#include <types.h>

typedef struct interrupt_frame interrupt_frame_t;

typedef struct
{
    u16 limit;
    u32 base;
} __attribute__((packed)) idtr_t;

typedef struct
{
    u16 isr_low;       // The lower 16 bits of the ISR's address
    u16 kernel_cs;     // The GDT segment selector that the CPU will load into CS before calling the ISR
    u8 reserved;       // Set to zero
    u8 attributes;     // Type and attributes
    u16 isr_high;      // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

void idt_init();
void idt_set_descriptor(u8 vector, void *isr, u8 flags);
