#pragma once

#include <stdint.h>
#include <stddef.h>

#define PAGE_FLAG_PRESENT 1
#define PAGE_FLAG_READ_WRITE (1 << 1)
#define PAGE_FLAG_ACCESSED (1 << 5)

void paging_init(void *page_directory, size_t identity_pages_count);
void paging_reset();
void paging_invalidate_all(void *page_directory);
void paging_enable(void *page_directory);
__attribute__((no_caller_saved_registers)) uint32_t* memory_virtual_to_pte(uintptr_t virtual_memory);
__attribute__((no_caller_saved_registers)) void paging_invalidate_page(uintptr_t address);
__attribute__((no_caller_saved_registers)) uintptr_t paging_read_cr2();
__attribute__((no_caller_saved_registers)) void paging_make_page_present(uintptr_t virtual_address, void *free_page_memory);
__attribute__((no_caller_saved_registers)) void paging_make_page_swapped(uintptr_t virtual_address);
__attribute__((no_caller_saved_registers)) void paging_increment_page_fault_counter();
__attribute__((no_caller_saved_registers)) void paging_reset_page_fault_counter();
uint32_t paging_get_page_fault_count();
