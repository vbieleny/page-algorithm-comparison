#include <idt.h>
#include <stddef.h>

__attribute__((aligned(0x10))) 
idt_entry_t idt[IDT_ENTRIES];

idtr_t idtr;

void idt_init()
{
    idtr.base = (uintptr_t) &idt[0];
    idtr.limit = (uint16_t) sizeof(idt_entry_t) * IDT_ENTRIES - 1;

    asm volatile("lidt %0" : : "m"(idtr));
}

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
    idt_entry_t *descriptor = &idt[vector];

    descriptor->isr_low = (uint32_t) isr & 0xffff;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t) isr >> 16;
    descriptor->reserved = 0;
}
