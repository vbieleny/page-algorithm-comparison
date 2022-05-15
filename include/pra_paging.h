/**
 * @file
 * @brief Handles paging related functionality, such as allocation or invalidation of pages.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "pra_attrs.h"

#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_READ_WRITE (1 << 1)
#define PAGE_FLAG_ACCESSED (1 << 5)
#define PAGE_FLAG_DIRTY (1 << 6)

/**
 * @brief Defined structure of a PTE (page table entry) for easier usage.
 */
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

/**
 * @brief Results for a single page fault.
 */
typedef struct
{
    bool victim;
    page_table_entry_t pte;
} page_fault_handler_result_t;

/**
 * @brief Paging statistics for whole test program.
 */
typedef struct
{
    uint32_t page_fault_without_victim;
    uint32_t page_fault_with_victim;
    uint32_t victim_accessed_count;
    uint32_t victim_dirty_count;
} page_fault_statistics_t;

/**
 * @brief Initializes identity pages in which kernel resides.
 * @param identity_pages_count number of identity pages to initialize
 */
void paging_initialize(size_t identity_pages_count);

/**
 * @brief Frees and invalidates all pages, and resets the paging statistics to prepare for new test program.
 */
void paging_reset_to_default();

/**
 * @brief Frees all page table that were allocated whilst allocating pages.
 */
void paging_free_all_page_tables();

/**
 * @brief Invalidates all pages by resetting the CR3 register.
 */
void paging_invalidate_all_pages();

/**
 * @brief Enables the paging in the CPU by setting the corresponding bit in CR0.
 */
void paging_enable();

/**
 * @brief Helper function to that converts a virtual memory to a PDE (page directory entry).
 * @return pointer to a PDE
 */
uint32_t* memory_virtual_to_pde(uintptr_t virtual_memory);

/**
 * @brief Helper function to that converts a virtual memory to a PTE (page table entry).
 * @return pointer to a PTE
 */
uint32_t* memory_virtual_to_pte(uintptr_t virtual_memory);

/**
 * @brief Invalidates single page by using INVLPG instruction.
 * @param address address inside of a page we want to invalidate
 */
void paging_invalidate_page(uintptr_t address);

/**
 * @brief Retrieves an address that was accessed when a page fault occurred.
 * @return virtual address of accessed memory
 */
uintptr_t paging_get_accessed_address();

/**
 * @brief Makes a page present for specified virtual memory. Allocates new page or page table if needed.
 */
void paging_make_page_present(uintptr_t virtual_address);

/**
 * @brief Makes a page not present.
 */
void paging_make_page_not_present(uintptr_t virtual_address);

/**
 * @brief Adds a single page fault result to the overall statistics.
 * @param result a result of a single page fault
 */
void paging_add_result_to_statistics(page_fault_handler_result_t *result);

/**
 * @brief Resets overall page fault statistics.
 */
void paging_reset_page_fault_statistics();

/**
 * @brief Returns the overall page fault statistics.
 * @return overall page fault statistics
 */
page_fault_statistics_t paging_get_page_fault_statistics();

/**
 * @brief Helper function that will fill the result by providing a victim PTE (page table entry)
 * @param result the result that will be filled
 * @param victim_pte victim PTE that will be used to fill the result
 */
void page_fault_handler_result_fill(page_fault_handler_result_t *result, page_table_entry_t *victim_pte);

/**
 * @brief Returns the PTE (page table entry) address from the PTE.
 * @return a page table entry address
 */
uint32_t* page_table_entry_address(page_table_entry_t *pte);
