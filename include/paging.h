#pragma once

#include <stdint.h>
#include <stddef.h>
#include <attrs.h>

#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_READ_WRITE (1 << 1)
#define PAGE_FLAG_ACCESSED (1 << 5)
#define PAGE_FLAG_DIRTY (1 << 6)

void paging_initialize(size_t identity_pages_count);
void paging_reset_to_default();
void paging_free_all_page_tables();
void paging_invalidate_all_pages();
void paging_enable();
NO_CALLER_SAVED_REGISTERS uint32_t* memory_virtual_to_pde(uintptr_t virtual_memory);
NO_CALLER_SAVED_REGISTERS uint32_t* memory_virtual_to_pte(uintptr_t virtual_memory);
NO_CALLER_SAVED_REGISTERS void paging_invalidate_page(uintptr_t address);
NO_CALLER_SAVED_REGISTERS uintptr_t paging_get_accessed_address();
NO_CALLER_SAVED_REGISTERS void paging_make_page_present(uintptr_t virtual_address);
NO_CALLER_SAVED_REGISTERS void paging_make_page_not_present(uintptr_t virtual_address);
NO_CALLER_SAVED_REGISTERS void paging_increment_page_fault_counter();
NO_CALLER_SAVED_REGISTERS void paging_reset_page_fault_counter();
uint32_t paging_get_page_fault_count();