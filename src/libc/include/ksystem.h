#pragma once

#include <attrs.h>
#include <stdint.h>
#include <idt.h>
#include <stdbool.h>

typedef enum
{
    TABLE_GDT, TABLE_IDT, TABLE_LDT
} descriptor_table_t;

typedef struct
{
    bool is_external;
    descriptor_table_t table;
    uint16_t index;
} selector_error_code_t;

NORETURN INTERRUPT void kernel_panic(interrupt_frame_t* frame, uint32_t error_code);
NORETURN NO_CALLER_SAVED_REGISTERS void halt();
NO_CALLER_SAVED_REGISTERS ALWAYS_INLINE selector_error_code_t parse_selector_error_code(uint32_t error_code);
