#pragma once

#include <attrs.h>
#include <stdint.h>

typedef struct interrupt_frame
{
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} interrupt_frame_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} PACKED idtr_t;

typedef struct
{
    uint16_t isr_low;       // The lower 16 bits of the ISR's address
    uint16_t kernel_cs;     // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t reserved;       // Set to zero
    uint8_t attributes;     // Type and attributes
    uint16_t isr_high;      // The higher 16 bits of the ISR's address
} PACKED idt_entry_t;

void idt_initialize();
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);
void interrupts_enable();
