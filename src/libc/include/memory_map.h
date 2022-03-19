#pragma once

#include <types.h>

typedef struct
{
    u32 base_low;
    u32 base_high;
    u32 length_low;
    u32 length_high;
    u32 type;
    u32 attributes;
} memory_map_entry_t;

extern memory_map_entry_t *memory_map;

void memory_map_print();
