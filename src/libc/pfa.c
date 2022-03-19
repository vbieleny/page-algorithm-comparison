#include <pfa.h>
#include <kmalloc.h>

typedef struct
{
    void *address;
    bool is_used;
} pfa_page_t;

static void *pfa_start_address;
static size_t pfa_size_in_pages;
static size_t pfa_allocation_limit;
static pfa_page_t *pages_memory;

void pfa_init(void *start_address, size_t size_in_pages)
{
    pfa_start_address = start_address;
    pfa_size_in_pages = size_in_pages;
    pages_memory = (pfa_page_t*) kernel_memory_allocate(pfa_size_in_pages * sizeof(pfa_page_t), 1);

    for (size_t i = 0; i < pfa_size_in_pages; i++, start_address += 0x1000)
    {
        pages_memory[i].address = start_address;
        pages_memory[i].is_used = false;
    }
}

void *pfa_allocate_page()
{
    for (size_t i = 0; i < pfa_size_in_pages; i++)
    {
        if (!pages_memory[i].is_used)
        {
            pages_memory[i].is_used = true;
            return pages_memory[i].address;
        }
    }
    return NULL;
}

void pfa_free_all()
{
    for (size_t i = 0; i < pfa_size_in_pages; i++)
        pages_memory[i].is_used = false;
}

void pfa_set_allocation_limit(size_t limit)
{
    pfa_allocation_limit = limit;
}

void* pfa_get_start_address()
{
    return pfa_start_address;
}

bool pfa_is_allocation_limit_reached()
{
    bool has_free_page = false;
    for (size_t i = 0; i < pfa_allocation_limit; i++)
    {
        if (!pages_memory[i].is_used)
        {
            has_free_page = true;
            break;
        }
    }
    return !has_free_page;
}
