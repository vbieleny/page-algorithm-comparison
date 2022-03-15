#include <kernel_malloc.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool is_available;
    size_t size;
} memory_control_block_t;

static uint8_t *last_valid_address;
static uint8_t *managed_memory_start;
static bool is_initialized;

void kernel_set_heap_start(void *heap_start)
{
    last_valid_address = (uint8_t*) heap_start;
    managed_memory_start = last_valid_address;
    is_initialized = true;
}

void* kernel_memory_allocate(size_t size)
{
    if (!is_initialized)
        return NULL;
    size += sizeof(memory_control_block_t);
    uint8_t *current_location = managed_memory_start;
    uint8_t *memory_location = NULL;
    while (current_location != last_valid_address)
    {
        memory_control_block_t *current_mcb = (memory_control_block_t*) current_location;
        if (current_mcb->is_available && current_mcb->size >= size)
        {
            current_mcb->is_available = false;
            memory_location = current_location;
            break;
        }
        current_location += current_mcb->size;
    }
    if (!memory_location)
    {
        memory_location = last_valid_address;
        last_valid_address += size;
        memory_control_block_t *memory_location_mcb = (memory_control_block_t*) memory_location;
        memory_location_mcb->is_available = false;
        memory_location_mcb->size = size;
    }
    return (void*) (memory_location + sizeof(memory_control_block_t));
}

void kernel_memory_free(void *address)
{
    memory_control_block_t *control_block = (memory_control_block_t*) ((uint8_t*) address - sizeof(memory_control_block_t));
    control_block->is_available = true;
}
