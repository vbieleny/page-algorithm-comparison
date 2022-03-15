#pragma once

#include <stddef.h>

void kernel_set_heap_start(void *heap_start);
void* kernel_memory_allocate(size_t size);
void kernel_memory_free(void *address);
