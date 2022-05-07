#pragma once

#include <stddef.h>

void kernel_memory_initialize(void *memory_start, size_t memory_size_bytes);
void* kernel_memory_allocate(size_t bytes, size_t alignment);
void* kernel_memory_reallocate(void *allocated_memory, size_t memory_new_size_bytes, size_t alignment);
void kernel_memory_free(void *allocated_memory);
size_t kernel_memory_get_free_mem();
size_t kernel_memory_get_free_blocks();
size_t kernel_memory_get_block_size();
