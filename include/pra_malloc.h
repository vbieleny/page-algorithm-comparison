/**
 * @file
 * @brief Header for user memory allocation, that allocates memory at random within defined allocation spread.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Initializes the memory that will be used by users. Called automatically by kernel.
 */
void user_memory_initialize();

/**
 * @brief Allocates new memory block of specified size within the allocation spread of currently running test program.
 * @param size size of memory block we want to allocate in bytes
 * @return pointer to allocated memory block
 */
void* user_memory_random_allocate(size_t size);

/**
 * @brief Frees all allocated memory by user. This is automatically called at the beginning of each test program.
 */
void user_memory_free_all();

/**
 * @brief Sets the available memory size for allocation. Acts as a software limit of memory size.
 * @param pages how many 4KiB pages the user memory allocator can allocate from
 */
void user_memory_set_memory_size(uint32_t pages);
