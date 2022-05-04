#include <paging.h>
#include <lib.h>
#include <pfa.h>
#include <pqueue.h>
#include <kmalloc.h>

static uint32_t *page_directory;
static uint32_t *identity_page_tables;
static uint32_t identity_pages_count;
static page_fault_statistics_t page_fault_statistics;

void paging_initialize(size_t pages_count)
{
    identity_pages_count = pages_count;
    page_directory = (uint32_t*) kernel_memory_allocate(1024 * sizeof(uint32_t), 0x1000);
    identity_page_tables = (uint32_t*) kernel_memory_allocate(identity_pages_count * sizeof(uint32_t), 0x1000);

    for (size_t i = 0; i < identity_pages_count / 1024; i++)
        page_directory[i] = (((uint32_t) identity_page_tables) + (i * 0x1000)) | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    for (size_t i = 0; i < identity_pages_count; i++)
        identity_page_tables[i] = (i * 0x1000) | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
}

void paging_reset_to_default()
{
    paging_free_all_page_tables();
    paging_invalidate_all_pages();
    paging_reset_page_fault_statistics();
}

void paging_free_all_page_tables()
{
    for (size_t i = identity_pages_count / 1024; i < 1024; i++)
    {
        if (page_directory[i] & PAGE_FLAG_PRESENT)
        {
            void *page_table_address = (void*) (page_directory[i] & ~0xfff);
            kernel_memory_free(page_table_address);
            page_directory[i] = 0;
        }
    }
}

void paging_invalidate_all_pages()
{
    asm volatile("mov cr3, eax" : : "a"(page_directory));
}

void paging_enable()
{
    paging_invalidate_all_pages();
    asm volatile(
    "mov eax, cr0\n"
    "or eax, 0x80000000\n"
    "mov cr0, eax\n"
    );
}

uint32_t* memory_virtual_to_pde(uintptr_t virtual_memory)
{
    uint16_t pde_index = (virtual_memory >> 22) & 0x3ff;
    return page_directory + pde_index;
}

uint32_t* memory_virtual_to_pte(uintptr_t virtual_memory)
{
    uint16_t pte_index = (virtual_memory >> 12) & 0x3ff;
    uint16_t pde_index = (virtual_memory >> 22) & 0x3ff;
    uint32_t *page_table_address = (uint32_t*) (page_directory[pde_index] & ~0xfff);
    return page_table_address + pte_index;
}

void paging_invalidate_page(uintptr_t address_in_page)
{
    asm volatile("invlpg [%0]" :: "r" (address_in_page) : "memory");
}

uintptr_t paging_get_accessed_address()
{
    uintptr_t cr2;
    asm volatile("mov %0, cr2" : "=r"(cr2));
    return cr2;
}

void paging_make_page_present(uintptr_t virtual_address)
{
    uint32_t *pde = memory_virtual_to_pde(virtual_address);
    if (!(*pde & PAGE_FLAG_PRESENT))
    {
        uint32_t *page_table = (uint32_t*) kernel_memory_allocate(1024 * sizeof(uint32_t), 0x1000);
        memset(page_table, 0, 1024 * sizeof(uint32_t));
        *pde = (uint32_t) page_table | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    }
    uint32_t *pte = memory_virtual_to_pte(virtual_address);
    if (*pte & ~0xfff)
        *pte |= (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    else
        *pte = (uint32_t) pfa_allocate_page() | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    page_entry_t page_entry = { virtual_address };
    page_queue_offer(page_entry);
}

void paging_make_page_not_present(uintptr_t virtual_address)
{
    page_table_entry_t *pte = (page_table_entry_t*) memory_virtual_to_pte(virtual_address);
    pte->present = 0;
    page_queue_poll();
}

void paging_add_result_to_statistics(page_fault_handler_result_t *result)
{
    if (result->victim)
    {
        page_fault_statistics.hard_page_faults++;
        if (result->pte.accessed)
            page_fault_statistics.victim_accessed_count++;
        if (result->pte.dirty)
            page_fault_statistics.victim_dirty_count++;
    }
    else
    {
        page_fault_statistics.soft_page_faults++;
    }
}

void paging_reset_page_fault_statistics()
{
    memset(&page_fault_statistics, 0, sizeof(page_fault_statistics));
}

page_fault_statistics_t paging_get_page_fault_statistics()
{
    return page_fault_statistics;
}

void page_fault_handler_result_fill(page_fault_handler_result_t *result, page_table_entry_t *victim_pte)
{
    result->victim = true;
    memcpy(&result->pte, victim_pte, sizeof(page_table_entry_t));
}

uint32_t* page_table_entry_address(page_table_entry_t *pte)
{
    return (uint32_t*) (pte->address << 12);
}
