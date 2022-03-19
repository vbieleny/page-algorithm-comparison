#pragma once

#include <types.h>
#include <stddef.h>

void* memory_random_allocate(size_t size);
void memory_free_all();
void set_max_pages_to_allocate(u32 max_pages);
