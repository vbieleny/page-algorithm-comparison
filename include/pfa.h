#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <attrs.h>

void pfa_initialize(void *start_address);
void *pfa_allocate_page();
void pfa_free_all_pages();
void* pfa_get_start_address();
void pfa_set_max_pages(size_t limit);
bool pfa_is_page_allocation_limit_reached();