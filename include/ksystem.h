#pragma once

#include <attrs.h>
#include <stdint.h>
#include <idt.h>
#include <stdbool.h>

typedef enum
{
    TABLE_GDT, TABLE_IDT, TABLE_LDT
} descriptor_table_e;

typedef struct
{
    bool is_external;
    descriptor_table_e table;
    uint16_t index;
} selector_error_code_t;

NORETURN void pra_kernel_panic_handler(uint32_t error_code);
NORETURN void halt();

ALWAYS_INLINE selector_error_code_t parse_selector_error_code(uint32_t error_code)
{
    selector_error_code_t selector_error_code;
    selector_error_code.is_external = error_code & 0b1;
    switch ((error_code >> 1) & 0b11)
    {
        case 0b00:
            selector_error_code.table = TABLE_GDT;
            break;
        case 0b01:
            selector_error_code.table = TABLE_IDT;
            break;
        case 0b10:
            selector_error_code.table = TABLE_LDT;
            break;
        case 0b11:
            selector_error_code.table = TABLE_IDT;
            break;
    }
    selector_error_code.index = (error_code >> 3) & 0x1fff;
    return selector_error_code;
}
