#include <pfa.h>
#include <stdint.h>
#include <stdbool.h>
#include <terminal.h>

typedef struct
{
    void *address;
    bool is_used;
} page_t;

#define MAX_PAGE_COUNT 128
#define MAX_SWAP_PAGE_COUNT 256

static page_t pages[MAX_PAGE_COUNT];
static page_t swap_pages[MAX_SWAP_PAGE_COUNT];

static size_t page_count_limit;

static void* pfa_allocate(page_t *pages_list, size_t size);

// start_address must be page-aligned (4K blocks)
void pfa_init(void *start_address, void *swap_start_address, size_t pages_limit)
{
    terminal_printf("PFA Init (0x%x, 0x%x, %d)\n", start_address, swap_start_address, pages_limit);
    page_count_limit = pages_limit;
    for (size_t i = 0; i < pages_limit; i++, start_address += 0x1000)
    {
        pages[i].address = start_address;
        pages[i].is_used = false;
    }
    for (size_t i = 0; i < MAX_SWAP_PAGE_COUNT; i++, swap_start_address += 0x1000)
    {
        swap_pages[i].address = swap_start_address;
        swap_pages[i].is_used = false;
    }
}

void* pfa_page_allocate()
{
    return pfa_allocate(pages, sizeof(pages) / sizeof(pages[0]));
}

void* pfa_swap_page_allocate()
{
    return pfa_allocate(swap_pages, sizeof(swap_pages) / sizeof(swap_pages[0]));;
}

void pfa_mark_swap_free(void *swap_address)
{
    for (int i = 0; i < sizeof(swap_pages) / sizeof(swap_pages[0]); i++)
    {
        if (swap_pages[i].address == swap_address)
            swap_pages[i].is_used = false;
    }
}

static void* pfa_allocate(page_t *pages_list, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (!pages_list[i].is_used)
        {
            pages_list[i].is_used = true;
            return pages_list[i].address;
        }
    }
    return NULL;
}

void* pfa_get_start_address()
{
    return pages[0].address;
}

size_t pfa_get_swap_page_count()
{
    return MAX_SWAP_PAGE_COUNT;
}
