/**
 * @file
 * @brief Functionality of a kernel memory allocator used for kernel code.
 */

#pragma once

#include <stddef.h>

/**
 * @brief Initializes a kernel memory allocator by providing a free memory block.
 * @param memory_start starting address of a free memory block
 * @param memory_size_bytes size of the free block in bytes
 */
void kernel_memory_initialize(void *memory_start, size_t memory_size_bytes);

/**
 * @brief Allocates a certain amount of memory with specified alignment.
 * @param bytes how many bytes to allocate
 * @param alignment the alignment of allocated memory
 * @return allocated memory
 */
void* kernel_memory_allocate(size_t bytes, size_t alignment);

/**
 * @brief Reallocates already allocated memory, effectivly changing its size.
 * @param allocated_memory existing allocated memory
 * @param memory_new_size_bytes new size of allocated memory
 * @param alignment the alignment of new allocated memory
 * @return new allocated memory
 */
void* kernel_memory_reallocate(void *allocated_memory, size_t memory_new_size_bytes, size_t alignment);

/**
 * @brief Frees allocated memory.
 * @param allocated_memory existing allocated memory
 */
void kernel_memory_free(void *allocated_memory);

/**
 * @brief Returns how many free bytes are available in the kernel allocator.
 * @return free memory in bytes
 */
size_t kernel_memory_get_free_mem();

/**
 * @brief Returns how many free blocks are available in the kernel allocator.
 * @return number of free blocks
 */
size_t kernel_memory_get_free_blocks();

/**
 * @brief Returns size of free block structure in which block information is stored.
 * @return size of free block structure
 */
size_t kernel_memory_get_block_size();
