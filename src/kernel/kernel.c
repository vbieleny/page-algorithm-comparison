#include <terminal.h>
#include <string.h>
#include <kernel.h>

void kernel_main()
{
    terminal_initialize();
    terminal_printf("Hello World!\n");
}
