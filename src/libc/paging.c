#include <paging.h>

static uint32_t *page_directory;
static uint32_t *page_tables;

void paging_init(void *page_directory_address, size_t identity_pages_count)
{
    page_directory = (uint32_t*) page_directory_address;
    page_tables = (uint32_t*) page_directory + 0x1000;

    for (size_t i = 0; i < 1024; i++)
        page_directory[i] = (((uint32_t) page_tables) + (i * 0x1000)) | 3;
    for (size_t i = 0; i < identity_pages_count; i++)
        page_tables[i] = (i * 0x1000) | 3;
}

uint32_t* memory_virtual_to_pte(uintptr_t virtual_memory)
{
    uint16_t pte_index = (virtual_memory >> 12) & 0x3ff;
    uint16_t pde_index = (virtual_memory >> 22) & 0x3ff;
    uint32_t *page_table_address = (uint32_t*) (page_directory[pde_index] & ~0xfff);
    return page_table_address + pte_index;
}

void paging_enable(void *page_directory);
void paging_invalidate_page(uintptr_t address);
uintptr_t paging_read_cr2();
