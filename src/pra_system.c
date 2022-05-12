#include "pra_io.h"
#include "pra_system.h"

void pra_kernel_panic_handler(uint32_t error_code)
{
    if (error_code)
    {
        selector_error_code_t selector_error_code = parse_selector_error_code(error_code);
        const char *format;
        switch (selector_error_code.table)
        {
            case TABLE_GDT:
                format = "GDT";
                break;
            case TABLE_IDT:
                format = "IDT";
                break;
            case TABLE_LDT:
                format = "LDT";
                break;
            default:
                format = "Unknown";
                break;
        }
        io_printf("\n============================================================");
        io_printf("\nKERNEL PANIC (External: %d, Table: %s, Index: %d)\n", selector_error_code.is_external, format, selector_error_code.index);
        io_printf("============================================================\n");
    }
    else
    {
        io_printf("\n============================================================\n");
        io_printf("\nKERNEL PANIC\n");
        io_printf("\n============================================================\n");
    }
    halt();
}

void halt()
{
    for (;;)
        asm volatile("hlt");
}
