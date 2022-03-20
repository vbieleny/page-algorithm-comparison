#pragma once

#include <stdint.h>
#include <stddef.h>

void user_memory_initialize();
void* user_memory_random_allocate(size_t size);
void user_memory_free_all();
void user_memory_set_memory_size(uint32_t pages);
