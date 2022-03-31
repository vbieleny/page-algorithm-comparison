#pragma once

#include <stdint.h>
#include <attrs.h>

#define PIC1 0x20
#define PIC2 0xa0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0c
#define ICW4_SFNM 0x10

#define IRQ0_TIMER 0

void pic_remap(uint8_t offset1, uint8_t offset2);
NO_CALLER_SAVED_REGISTERS void pic_send_eoi(uint8_t irq);
void irq_set_mask(uint8_t irq_line);
void irq_set_mask_all();
void irq_clear_mask(uint8_t irq_line);
