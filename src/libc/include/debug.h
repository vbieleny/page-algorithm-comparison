#pragma once

inline void debug_bochs_breakpoint()
{
    asm volatile("xchg bx, bx");
}
