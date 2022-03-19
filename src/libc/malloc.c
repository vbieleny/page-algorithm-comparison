#include <malloc.h>
#include <stdbool.h>
#include <random.h>
#include <pfa.h>
#include <kmalloc.h>

typedef struct
{
    u32 address;
    size_t size;
} memory_block_t;

static const size_t DEFAULT_BLOCKS_SIZE = 1024;

static u32 max_pages_to_allocate = 0;

static memory_block_t *used_blocks;
static size_t used_block_size = 0;
static size_t total_block_size = 0;

static void memory_reallocate_used_blocks(size_t size);
static bool memory_is_used(memory_block_t block);
static void memory_make_used(memory_block_t block);

static void memory_reallocate_used_blocks(size_t size)
{
    used_blocks = kernel_memory_reallocate(used_blocks, size * sizeof(memory_block_t), 1);
    total_block_size = size;
}

static bool memory_is_used(memory_block_t block)
{
    for (size_t i = 0; i < used_block_size; i++)
    {
        if (block.address >= used_blocks[i].address + used_blocks[i].size)
            continue;
        if (block.address < used_blocks[i].address && block.address + block.size < used_blocks[i].address)
            continue;
        return true;
    }
    return false;
}

static void memory_make_used(memory_block_t block)
{
    if (used_block_size >= total_block_size)
        memory_reallocate_used_blocks(total_block_size * 2);
    used_blocks[used_block_size++] = block;
}

void memory_init()
{
    used_blocks = kernel_memory_allocate(DEFAULT_BLOCKS_SIZE * sizeof(memory_block_t), 1);
    total_block_size = DEFAULT_BLOCKS_SIZE;
}

void* memory_random_allocate(size_t size)
{
    uintptr_t start_address = (uintptr_t) pfa_get_start_address();
    uintptr_t end_address = start_address + (0x1000 * max_pages_to_allocate);
    memory_block_t random_block = { .size = size };
    u16 counter = 0;
    do
    {
        random_block.address = random_between(start_address, end_address);
        counter++;
        if (counter >= 1024)
            return NULL;
    } while (memory_is_used(random_block));
    memory_make_used(random_block);
    return (void*) random_block.address;
}

void memory_free_all()
{
    used_block_size = 0;
}

void memory_set_max_pages_to_allocate(u32 max_pages)
{
    max_pages_to_allocate = max_pages;
}
