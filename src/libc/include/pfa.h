#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <memory_map.h>

typedef struct
{
    void *address;
    bool used;
} page_t;

#define MAX_PAGE_COUNT (128)
#define MAX_SWAP_PAGE_COUNT ((MAX_PAGE_COUNT) * 2)

extern page_t pages[MAX_PAGE_COUNT];
extern page_t swap_pages[MAX_SWAP_PAGE_COUNT];

void init_pages(void *start_address, memory_map_entry_t *memory_map, size_t limit);
void* allocate_page();
void* allocate_swap_page();
void free_all_pages();
size_t get_pages_limit();
