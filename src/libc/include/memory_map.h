#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t base_low;
    uint32_t base_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
    uint32_t attributes;
} memory_map_entry_t;

#define MEMORY_MAP_ADDRESS (0x7e00)
#define MEMORY_MAP_COUNT (0x9000)

extern memory_map_entry_t *memory_map;

void print_memory_map();
