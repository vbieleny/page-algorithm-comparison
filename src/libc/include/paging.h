#pragma once

#include <stdint.h>

inline void paging_enable(uint32_t *page_directory)
{
    asm volatile("mov cr3, eax" : : "a"(page_directory));
    asm volatile(
        "mov eax, cr0\n"
        "or eax, 0x80000000\n"
        "mov cr0, eax\n"
    );
}

inline void paging_invalidate_page(uintptr_t address)
{
   asm volatile("invlpg [%0]" :: "r" (address) : "memory");
}

inline uintptr_t paging_read_cr2()
{
    uintptr_t cr2;
    asm volatile("mov %0, cr2" : "=r"(cr2));
    return cr2;
}
