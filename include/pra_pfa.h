/**
 * @file
 * @brief Provides PFA (page frame allocation) functionality.
 */

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "pra_attrs.h"

/**
 * @brief Initializes PFA (page frame allocator) to specified start address from which it will allocate pages.
 */
void pfa_initialize(void *start_address);

/**
 * @brief Allocates a new page in order.
 * Will return new page event if limit was reached. If you want to respect the limit,
 * you have to check it manually via pfa_is_page_allocation_limit_reached() function.
 * @see pfa_is_page_allocation_limit_reached()
 * @return address of the new page in memory
 */
void *pfa_allocate_page();

/**
 * @brief Frees all previously allocated pages.
 */
void pfa_free_all_pages();

/**
 * @brief Returns start address of the PFA (page frame allocator) from which it allocates pages.
 * @return start address of PFA
 */
void* pfa_get_start_address();

/**
 * @brief Sets the maximum allocation limit for pages.
 * @param limit how many pages it can allocate
 */
void pfa_set_max_pages(size_t limit);

/**
 * @brief Checks whether the allocation limit is reached.
 * @return true if the limit was reached, otherwise false
 */
bool pfa_is_page_allocation_limit_reached();
