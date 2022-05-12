#include "pra_debug.h"

void pra_debug_bochs_breakpoint()
{
    asm volatile("xchg bx, bx");
}
