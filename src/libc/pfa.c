#include <pfa.h>

page_t pages[MAX_PAGE_COUNT];
page_t swap_pages[MAX_SWAP_PAGE_COUNT];

static size_t pages_limit;

static void init_swap_pages(void *start_address, memory_map_entry_t *memory_map);

// start_address must be page-aligned (4K blocks)
void init_pages(void *start_address, memory_map_entry_t *memory_map, size_t limit)
{
    // TODO(vbieleny): Check in memory map if there is enough available memory
    pages_limit = limit;
    for (int i = 0; i < pages_limit; i++, start_address += 0x1000)
    {
        pages[i].address = start_address;
        pages[i].used = false;
    }
    init_swap_pages(start_address, memory_map);
}

static void init_swap_pages(void *start_address, memory_map_entry_t *memory_map)
{
    for (int i = 0; i < sizeof(swap_pages) / sizeof(swap_pages[0]); i++, start_address += 0x1000)
    {
        swap_pages[i].address = start_address;
        swap_pages[i].used = false;
    }
}

void* allocate_page()
{
    for (int i = 0; i < sizeof(pages) / sizeof(pages[0]); i++)
    {
        if (!pages[i].used)
        {
            pages[i].used = true;
            return pages[i].address;
        }
    }
    return NULL;
}

void free_all_pages()
{
    for (int i = 0; i < sizeof(pages) / sizeof(pages[0]); i++)
        pages[i].used = false;
}

size_t get_pages_limit()
{
    return pages_limit;
}
