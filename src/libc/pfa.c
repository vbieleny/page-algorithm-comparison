#include <pfa.h>
#include <kmalloc.h>
#include <stdint.h>

static void *pfa_start_address;
static void *pfa_next_page_to_allocate;
static size_t pfa_allocation_limit;

void pfa_initialize(void *start_address)
{
    pfa_start_address = start_address;
    pfa_next_page_to_allocate = pfa_start_address;
}

void *pfa_allocate_page()
{
    void *allocated_page = pfa_next_page_to_allocate;
    pfa_next_page_to_allocate += 0x1000;
    return allocated_page;
}

void pfa_free_all_pages()
{
    pfa_next_page_to_allocate = pfa_start_address;
}

void* pfa_get_start_address()
{
    return pfa_start_address;
}

void pfa_set_page_allocation_limit(size_t limit)
{
    pfa_allocation_limit = limit;
}

bool pfa_is_page_allocation_limit_reached()
{
    size_t allocated_pages_memory_size = (size_t) ((uint8_t*) pfa_next_page_to_allocate - (uint8_t*) pfa_start_address);
    return allocated_pages_memory_size / 0x1000 >= pfa_allocation_limit;
}
