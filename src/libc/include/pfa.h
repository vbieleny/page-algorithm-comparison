#pragma once

#include <stddef.h>

void pfa_init(void *start_address, void *swap_start_address, size_t pages_limit);
__attribute__((no_caller_saved_registers)) void* pfa_page_allocate();
void* pfa_swap_page_allocate();
void pfa_mark_swap_free(void *swap_address);
void* pfa_get_start_address();
size_t pfa_get_swap_page_count();
