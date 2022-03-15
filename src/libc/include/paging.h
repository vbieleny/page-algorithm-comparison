#pragma once

#include <stdint.h>
#include <stddef.h>

void paging_init(void *page_directory, size_t identity_pages_count);
__attribute__((no_caller_saved_registers)) uint32_t* memory_virtual_to_pte(uintptr_t virtual_memory);

inline void paging_enable(void *page_directory)
{
    asm volatile("mov cr3, eax" : : "a"(page_directory));
    asm volatile(
        "mov eax, cr0\n"
        "or eax, 0x80000000\n"
        "mov cr0, eax\n"
    );
}

__attribute__((no_caller_saved_registers))
inline void paging_invalidate_page(uintptr_t address)
{
   asm volatile("invlpg [%0]" :: "r" (address) : "memory");
}

__attribute__((no_caller_saved_registers))
inline uintptr_t paging_read_cr2()
{
    uintptr_t cr2;
    asm volatile("mov %0, cr2" : "=r"(cr2));
    return cr2;
}
