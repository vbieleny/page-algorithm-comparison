#include "pra_idt.h"
#include <stddef.h>
#include "pra_attrs.h"
#include "pra_io.h"

#define IDT_ENTRIES_COUNT 256

static const uint8_t KERNEL_CODE_SELECTOR = 0x08;

PRA_ALIGNED(0x10) static idt_entry_t idt[IDT_ENTRIES_COUNT];
static idtr_t idtr;

void idt_initialize()
{
    idtr.base = (uintptr_t) &idt[0];
    idtr.limit = (uint16_t) (sizeof(idt_entry_t) * IDT_ENTRIES_COUNT - 1);
    asm volatile("lidt %0"
                 :
                 : "m"(idtr));
}

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
    idt_entry_t *descriptor = &idt[vector];
    descriptor->isr_low = (uintptr_t) isr & 0xffff;
    descriptor->kernel_cs = KERNEL_CODE_SELECTOR;
    descriptor->attributes = flags;
    descriptor->isr_high = (uintptr_t) isr >> 16;
    descriptor->reserved = 0;
}

void set_interrupts_enabled(bool enable)
{
    if (enable)
        asm volatile("sti");
    else
        asm volatile("cli");
}

bool are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile("pushf\n\t"
                 "pop %0" : "=g"(flags));
    return flags & (1 << 9);
}

void nmi_set_enabled(bool enable)
{
    if (enable)
    {
        io_out_byte(0x70, io_in_byte(0x70) & 0x7F);
        io_in_byte(0x71);
    }
    else
    {
        io_out_byte(0x70, io_in_byte(0x70) | 0x80);
        io_in_byte(0x71);
    }
}
