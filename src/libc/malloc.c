#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>
#include <random.h>
#include <pfa.h>

typedef struct
{
    uint32_t address;
    size_t size;
} memory_block_t;

#define MAX_ALLOCATION_BLOCKS (1024 * 16)

static const uint8_t MAX_PAGES_TO_ALLOCATE = 8;

static memory_block_t used_blocks[MAX_ALLOCATION_BLOCKS];
static size_t used_block_size = 0;

static bool memory_is_used(memory_block_t block);
static void memory_make_used(memory_block_t block);

static bool memory_is_used(memory_block_t block)
{
    for (size_t i = 0; i < sizeof(used_blocks) / sizeof(used_blocks[0]); i++)
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
    used_blocks[used_block_size++] = block;
}

void* memory_random_allocate(size_t size)
{
    uintptr_t start_address = (uintptr_t) pfa_get_start_address();
    uintptr_t end_address = start_address + (0x1000 * MAX_PAGES_TO_ALLOCATE);
    memory_block_t random_block = { .size = size };
    uint16_t counter = 0;
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
