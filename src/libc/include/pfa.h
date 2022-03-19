#pragma once

#include <stddef.h>
#include <stdbool.h>

void pfa_init(void *start_address, size_t size_in_pages);
__attribute__((no_caller_saved_registers)) void *pfa_allocate_page();
void pfa_free_all();
void pfa_set_allocation_limit(size_t limit);
void* pfa_get_start_address();
__attribute__((no_caller_saved_registers)) bool pfa_is_allocation_limit_reached();
