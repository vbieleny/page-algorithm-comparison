#include <debug.h>

void debug_bochs_breakpoint()
{
    asm volatile("xchg bx, bx");
}
