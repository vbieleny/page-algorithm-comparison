#include <stdint.h>
#include <stdbool.h>
#include <lib.h>
#include <pfa.h>
#include <random.h>

typedef struct
{
    uint32_t address;
    size_t size;
} allocation_block_t;

#define SPREAD_PAGES 8

static allocation_block_t used_blocks[1024 * 16];
static size_t used_block_size = 0;

static bool is_used(allocation_block_t block);
static void make_used(allocation_block_t block);

void* memmove(void *destination, const void *source, size_t length)
{
    asm volatile("rep movsb"
                : "=D" (destination),
                  "=S" (source),
                  "=c" (length)
                : "0" (destination),
                  "1" (source),
                  "2" (length)
                : "memory");
    return destination;
}

void* memmove_debug(void *destination, const void *source, size_t length)
{
    uint8_t *dest = destination;
    const uint8_t *src = source;
    for (size_t i = 0; i < length; i++)
        dest[i] = src[i];
    return destination;
}

void* rmalloc(size_t size)
{
    uint32_t start_address = (uint32_t) pages[0].address;
    uint32_t end_address = start_address + (0x1000 * SPREAD_PAGES);
    allocation_block_t random_block = { .size = size };
    uint16_t counter = 0;
    do
    {
        random_block.address = rand_between(start_address, end_address);
        counter++;
        if (counter >= 1024)
            return NULL;
    }
    while (is_used(random_block));
    make_used(random_block);
    return (void*) random_block.address;
}

static bool is_used(allocation_block_t block)
{
    for (int i = 0; i < sizeof(used_blocks) / sizeof(used_blocks[0]); i++)
    {
        if (block.address >= used_blocks[i].address + used_blocks[i].size)
            continue;
        if (block.address < used_blocks[i].address && block.address + block.size < used_blocks[i].address)
            continue;
        return true;
    }
    return false;
}

static void make_used(allocation_block_t block)
{
    used_blocks[used_block_size++] = block;
}
