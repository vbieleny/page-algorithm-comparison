#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <attrs.h>

void pfa_init(void *start_address, size_t size_in_pages);
NO_CALLER_SAVED_REGISTERS void *pfa_allocate_page();
void pfa_free_all();
void pfa_set_allocation_limit(size_t limit);
void* pfa_get_start_address();
NO_CALLER_SAVED_REGISTERS bool pfa_is_allocation_limit_reached();
