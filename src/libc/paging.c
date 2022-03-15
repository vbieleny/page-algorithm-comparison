#include <paging.h>
#include <lib.h>
#include <pfa.h>
#include <page_queue.h>

static uint32_t *page_directory;
static uint32_t *page_tables;
static uint32_t page_fault_counter;

void paging_init(void *page_directory_address, size_t identity_pages_count)
{
    page_directory = (uint32_t*) page_directory_address;
    page_tables = (uint32_t*) page_directory + 0x1000;

    for (size_t i = 0; i < 1024; i++)
        page_directory[i] = (((uint32_t) page_tables) + (i * 0x1000)) | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    for (size_t i = 0; i < identity_pages_count; i++)
        page_tables[i] = (i * 0x1000) | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
}

void paging_enable(void *page_directory)
{
    asm volatile("mov cr3, eax" : : "a"(page_directory));
    asm volatile(
    "mov eax, cr0\n"
    "or eax, 0x80000000\n"
    "mov cr0, eax\n"
    );
}

uint32_t* memory_virtual_to_pte(uintptr_t virtual_memory)
{
    uint16_t pte_index = (virtual_memory >> 12) & 0x3ff;
    uint16_t pde_index = (virtual_memory >> 22) & 0x3ff;
    uint32_t *page_table_address = (uint32_t*) (page_directory[pde_index] & ~0xfff);
    return page_table_address + pte_index;
}

void paging_invalidate_page(uintptr_t address)
{
    asm volatile("invlpg [%0]" :: "r" (address) : "memory");
}

uintptr_t paging_read_cr2()
{
    uintptr_t cr2;
    asm volatile("mov %0, cr2" : "=r"(cr2));
    return cr2;
}

void paging_make_page_present(uintptr_t virtual_address, void *free_page_memory)
{
    uint32_t *pte = memory_virtual_to_pte(virtual_address);
    uint32_t swap_address = (*pte) & ~0xfff;
    *pte = (uint32_t) free_page_memory | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    if (swap_address)
    {
        memory_move((void*) (virtual_address & ~0xfff), (void*) swap_address, 0x1000);
        pfa_mark_swap_free((void*) swap_address);
    }
    page_entry_t page_entry = { virtual_address };
    page_queue_offer(page_entry);
}

void paging_make_page_swapped(uintptr_t virtual_address)
{
    void *swap_page_memory = pfa_swap_page_allocate();
    memory_move(swap_page_memory, (void*) (virtual_address & ~0xfff), 0x1000);
    uint32_t *pte = memory_virtual_to_pte(virtual_address);
    *pte = (uint32_t) swap_page_memory | 2;
    page_queue_poll();
}

void paging_increment_page_fault_counter()
{
    page_fault_counter++;
}

uint32_t paging_get_page_fault_count()
{
    return page_fault_counter;
}
