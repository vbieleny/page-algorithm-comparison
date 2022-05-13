/**
 * @file
 * @brief Contains IDT and interrupts initialization and management.
 */

#pragma once

#include "pra_attrs.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t limit;
    uint32_t base;
} PRA_PACKED idtr_t;

typedef struct
{
    uint16_t isr_low;       // The lower 16 bits of the ISR's address
    uint16_t kernel_cs;     // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t reserved;       // Set to zero
    uint8_t attributes;     // Type and attributes
    uint16_t isr_high;      // The higher 16 bits of the ISR's address
} PRA_PACKED idt_entry_t;

/**
 * @brief Initializes IDT by loading the IDT via LIDT instruction. Must be use to for interrupts to work properly. Called automatically by kernel.
 */
void idt_initialize();

/**
 * @brief Sets interrupt descriptor to handle specified interrupt via ISR.
 * @param vector which interrupt to set
 * @param isr function that will be called when the interrupt happens
 * @param flags flags for interrupt, usually 0x8e for interrupt gates and 0xfe for trap gates
 */
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);

/**
 * @brief Enables or disables interrupts via STI or CLI instructions.
 * @param enable whether to enable or disable interrupts
 */
void set_interrupts_enabled(bool enable);

/**
 * @brief Returns whether the interrupts are enabled by checking the IF in FLAGS register.
 * @return whether the interrupts are enabled
 */
bool are_interrupts_enabled();

/**
 * @brief Enables or disables NMI (non-maskable interrupts).
 * @param enable whether to enable or disable NMI
 */
void nmi_set_enabled(bool enable);
