#include <idt.h>
#include <stddef.h>
#include <attrs.h>
#include <io.h>

#define IDT_ENTRIES_COUNT 256

static const uint8_t KERNEL_CODE_SELECTOR = 0x08;

ALIGNED(0x10) static idt_entry_t idt[IDT_ENTRIES_COUNT];
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

void interrupts_enable()
{
    asm volatile("sti");
}

void nmi_enable()
{
    io_out_byte(0x70, io_in_byte(0x70) & 0x7F);
    io_in_byte(0x71);
}

void nmi_disable()
{
    io_out_byte(0x70, io_in_byte(0x70) | 0x80);
    io_in_byte(0x71);
}
