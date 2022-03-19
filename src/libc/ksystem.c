#include <ksystem.h>
#include <io.h>

void kernel_panic(interrupt_frame_t *frame, u32 error_code)
{
    io_printf(DEFAULT_STREAM, "Kernel Panic (Error Code: 0x%x)\n", error_code);
    halt();
}

void halt()
{
    for (;;)
        asm volatile("hlt");
}
