/**
 * @file
 * @brief PIC (programmable interrupt controller) handling and IRQ (interrupt request) masking.
 */

#pragma once

#include <stdint.h>
#include "pra_attrs.h"

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

/**
 * @brief Remaps the PIC IRQs to specified offsets.
 * This is used mostly to remap the hardware interrupts that are by default
 * overlapping with exceptions.
 * @param offset1 where to put first 8 interrupts
 * @param offset2 where to put second 8 interrupts
 */
void pic_remap(uint8_t offset1, uint8_t offset2);

/**
 * @brief Send an EOI (end-of-interrupt) to the PIC. Used in IRQs that require EOI to be able to handle next interrupt.
 */
void pic_send_eoi(uint8_t irq);

/**
 * @brief Sets the mask for a specified IRQ. This disables the IRQ from sending interrupts.
 * @param irq_line which IRQ to disable
 */
void irq_set_mask(uint8_t irq_line);

/**
 * @brief Sets the mask for all interrupts. Use if you want to disable all hardware interrupts.
 */
void irq_set_mask_all();

/**
 * @brief Clears the mask for a specified IRQ. This allows the IRQ to send interrupts.
 * @param irq_line which IRQ to enable
 */
void irq_clear_mask(uint8_t irq_line);
