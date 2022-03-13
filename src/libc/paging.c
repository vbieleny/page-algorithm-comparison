#include <paging.h>

void enable_paging(uint32_t *page_directory)
{
    asm volatile("mov cr3, eax" : : "a"(page_directory));
    asm volatile(
        "mov eax, cr0\n"
        "or eax, 0x80000000\n"
        "mov cr0, eax\n"
    );
}

inline void invalidate_page(uint32_t address)
{
   asm volatile("invlpg [%0]" :: "r" (address) : "memory");
}

uint32_t read_cr2()
{
    uint32_t cr2;
    asm volatile("mov %0, cr2" : "=r"(cr2));
    return cr2;
}
