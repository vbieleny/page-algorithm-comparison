#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "pra_attrs.h"

#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_READ_WRITE (1 << 1)
#define PAGE_FLAG_ACCESSED (1 << 5)
#define PAGE_FLAG_DIRTY (1 << 6)

typedef struct
{
    uint32_t present : 1;
    uint32_t read_write : 1;
    uint32_t user_supervisor : 1;
    uint32_t write_through : 1;
    uint32_t cache_disable : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t page_attribute_table : 1;
    uint32_t global : 1;
    uint32_t available : 3;
    uint32_t address : 20;
} PRA_PACKED page_table_entry_t;

typedef struct
{
    bool victim;
    page_table_entry_t pte;
} page_fault_handler_result_t;

typedef struct
{
    uint32_t soft_page_faults;
    uint32_t hard_page_faults;
    uint32_t victim_accessed_count;
    uint32_t victim_dirty_count;
} page_fault_statistics_t;

void paging_initialize(size_t identity_pages_count);
void paging_reset_to_default();
void paging_free_all_page_tables();
void paging_invalidate_all_pages();
void paging_enable();
uint32_t* memory_virtual_to_pde(uintptr_t virtual_memory);
uint32_t* memory_virtual_to_pte(uintptr_t virtual_memory);
void paging_invalidate_page(uintptr_t address);
uintptr_t paging_get_accessed_address();
void paging_make_page_present(uintptr_t virtual_address);
void paging_make_page_not_present(uintptr_t virtual_address);
void paging_add_result_to_statistics(page_fault_handler_result_t *result);
void paging_reset_page_fault_statistics();
page_fault_statistics_t paging_get_page_fault_statistics();

void page_fault_handler_result_fill(page_fault_handler_result_t *result, page_table_entry_t *victim_pte);
uint32_t* page_table_entry_address(page_table_entry_t *pte);
