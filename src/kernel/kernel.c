#include <kernel.h>
#include <terminal.h>

extern uint32_t kernel_end;

void kernel_main()
{
    terminal_initialize();
    terminal_printf("Hello, World!\n");
}
