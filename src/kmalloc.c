#include <kmalloc.h>
#include <lib.h>
#include <stdint.h>

typedef struct free_block
{
    size_t size;
    struct free_block *next;
} free_block_t;

static free_block_t *root_block;

void kernel_memory_initialize(void *memory_start, size_t memory_size_bytes)
{
    root_block = (free_block_t*) memory_start;
    root_block->size = memory_size_bytes;
}

void* kernel_memory_allocate(size_t bytes, size_t alignment)
{
    free_block_t *block = root_block;
    free_block_t *previous_block = NULL;
    for (; block; previous_block = block, block = block->next)
    {
        for (uint8_t *next_aligned_address = (void*) ((uintptr_t) ((uint8_t*) block + sizeof(free_block_t) + alignment - 1) & ~(uintptr_t) (alignment - 1));; next_aligned_address += alignment)
        {
            if (next_aligned_address + bytes < (uint8_t*) block + block->size)
            {
                if (next_aligned_address - sizeof(free_block_t) > (uint8_t*) block)
                {
                    size_t block_size = (size_t) ((next_aligned_address - sizeof(free_block_t)) - (uint8_t*) block);
                    if (block_size < sizeof(free_block_t))
                        continue;
                    size_t next_block_size = (size_t) ((uint8_t*) block + block->size - (next_aligned_address + bytes));
                    if (next_block_size < sizeof(free_block_t))
                        break;
                    free_block_t *next_block = (free_block_t*) (next_aligned_address + bytes);
                    next_block->size = next_block_size;
                    block->size = block_size;
                    next_block->next = block->next;
                    block->next = next_block;
                }
                else if (next_aligned_address - sizeof(free_block_t) == (uint8_t*) block)
                {
                    size_t block_size = (size_t) (((uint8_t*) block + block->size) - (next_aligned_address + bytes));
                    free_block_t *next_block = block->next;
                    block = (free_block_t*) (next_aligned_address + bytes);
                    block->size = block_size;
                    block->next = next_block;
                    if (previous_block)
                        previous_block->next = block;
                    else
                        root_block = block;
                }
                else
                {
                    break;
                }
                *((size_t*) (next_aligned_address - sizeof(free_block_t))) = bytes;
                return next_aligned_address;
            }
            else if (next_aligned_address + bytes == ((uint8_t*) block + block->size))
            {
                if (next_aligned_address - sizeof(free_block_t) > (uint8_t*) block)
                {
                    block->size = (next_aligned_address - sizeof(free_block_t)) - (uint8_t*) block;
                }
                else if (next_aligned_address - sizeof(free_block_t) == (uint8_t*) block)
                {
                    if (previous_block)
                        previous_block->next = block->next;
                    else
                        root_block = block->next;
                }
                else
                {
                    break;
                }
                *((size_t*) (next_aligned_address - sizeof(free_block_t))) = bytes;
                return next_aligned_address;
            }
            else
            {
                break;
            }
        }
    }
    return NULL;
}

void* kernel_memory_reallocate(void *allocated_memory, size_t memory_new_size_bytes, size_t alignment)
{
    uint8_t *allocated_address = (uint8_t*) allocated_memory;
    size_t allocated_size = *((size_t*) (allocated_address - sizeof(free_block_t)));
    uint8_t *new_allocated_memory = (uint8_t*) kernel_memory_allocate(memory_new_size_bytes, alignment);
    size_t new_size = allocated_size < memory_new_size_bytes ? allocated_size : memory_new_size_bytes;
    pra_memcpy(new_allocated_memory, allocated_memory, new_size);
    kernel_memory_free(allocated_memory);
    return new_allocated_memory;
}

void kernel_memory_free(void *allocated_memory)
{
    uint8_t *allocated_address = (uint8_t*) allocated_memory;
    size_t allocated_size = *((size_t*) (allocated_address - sizeof(free_block_t))) + sizeof(free_block_t);
    free_block_t *allocated_block = (free_block_t*) (allocated_address - sizeof(free_block_t));
    allocated_block->size = allocated_size;

    free_block_t *block = root_block;
    free_block_t *previous_block = NULL;
    for (; block; previous_block = block, block = block->next)
    {
        if (block > allocated_block)
        {
            if (previous_block)
                previous_block->next = allocated_block;
            else
                root_block = allocated_block;
            allocated_block->next = block;
            break;
        }
    }
    if (!block)
    {
        if (previous_block)
            previous_block->next = allocated_block;
        else
            root_block = allocated_block;
    }

    previous_block = NULL;
    for (block = root_block; block; previous_block = block, block = block->next)
    {
        if (!previous_block)
            continue;
        if ((uint8_t*) previous_block + previous_block->size == (uint8_t*) block)
        {
            previous_block->next = block->next;
            previous_block->size += block->size;
            block = previous_block;
        }
    }
}

size_t kernel_memory_get_free_mem()
{
    size_t free_memory = 0;
    for (free_block_t *block = root_block; block; block = block->next)
        free_memory += block->size;
    return free_memory;
}

size_t kernel_memory_get_free_blocks()
{
    size_t free_blocks = 0;
    for (free_block_t *block = root_block; block; block = block->next)
        free_blocks++;
    return free_blocks;
}

size_t kernel_memory_get_block_size()
{
    return sizeof(free_block_t);
}
