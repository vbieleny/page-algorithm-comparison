#include <pfa.h>

page_t pages[MAX_PAGE_COUNT];
page_t swap_pages[MAX_SWAP_PAGE_COUNT];

static size_t pages_limit;

static void* allocate(page_t *pages_list, size_t size);

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
}

void init_swap_pages(void *start_address, memory_map_entry_t *memory_map)
{
    for (int i = 0; i < MAX_SWAP_PAGE_COUNT; i++, start_address += 0x1000)
    {
        swap_pages[i].address = start_address;
        swap_pages[i].used = false;
    }
}

static void* allocate(page_t *pages_list, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (!pages_list[i].used)
        {
            pages_list[i].used = true;
            return pages_list[i].address;
        }
    }
    return NULL;
}

void* allocate_page()
{
    return allocate(pages, sizeof(pages) / sizeof(pages[0]));
}

void* allocate_swap_page()
{
    return allocate(swap_pages, sizeof(swap_pages) / sizeof(swap_pages[0]));
}

void mark_swap_free(void *swap_address)
{
    for (int i = 0; i < sizeof(swap_pages) / sizeof(swap_pages[0]); i++)
    {
        if (swap_pages[i].address == swap_address)
            swap_pages[i].used = false;
    }
}

void free_all_pages()
{
    for (int i = 0; i < sizeof(pages) / sizeof(pages[0]); i++)
        pages[i].used = false;
    for (int i = 0; i < sizeof(swap_pages) / sizeof(swap_pages[0]); i++)
        swap_pages[i].used = false;
}

size_t get_pages_limit()
{
    return pages_limit;
}
