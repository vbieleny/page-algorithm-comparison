#include <io.h>
#include <ksystem.h>

void kernel_panic(interrupt_frame_t *frame, uint32_t error_code)
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
        io_printf("Kernel Panic (External: %d, Table: %s, Index: %d)\n", selector_error_code.is_external, format, selector_error_code.index);
    }
    else
        io_printf("Kernel Panic\n");
    halt();
}

void halt()
{
    for (;;)
        asm volatile("hlt");
}
