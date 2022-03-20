#pragma once

#include <stdint.h>

typedef struct interrupt_frame interrupt_frame_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

typedef struct
{
    uint16_t isr_low;       // The lower 16 bits of the ISR's address
    uint16_t kernel_cs;     // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t reserved;       // Set to zero
    uint8_t attributes;     // Type and attributes
    uint16_t isr_high;      // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

void idt_initialize();
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);
