#include <paging.h>
#include <lib.h>
#include <pfa.h>
#include <page_queue.h>
#include <kmalloc.h>

static u32 *page_directory;
static u32 *identity_page_tables;
static u32 page_fault_counter;
static u32 identity_pages_count;

void paging_init(size_t pages_count)
{
    identity_pages_count = pages_count;
    page_directory = (u32*) kernel_memory_allocate(1024 * sizeof(u32), 0x1000);
    identity_page_tables = (u32*) kernel_memory_allocate(identity_pages_count * sizeof(u32), 0x1000);

    for (size_t i = 0; i < identity_pages_count / 1024; i++)
        page_directory[i] = (((u32) identity_page_tables) + (i * 0x1000)) | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    for (size_t i = 0; i < identity_pages_count; i++)
        identity_page_tables[i] = (i * 0x1000) | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
}

void paging_reset()
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
    paging_invalidate_all();
}

void paging_invalidate_all()
{
    asm volatile("mov cr3, eax" : : "a"(page_directory));
}

void paging_enable()
{
    paging_invalidate_all();
    asm volatile(
    "mov eax, cr0\n"
    "or eax, 0x80000000\n"
    "mov cr0, eax\n"
    );
}

u32* memory_virtual_to_pde(uintptr_t virtual_memory)
{
    u16 pde_index = (virtual_memory >> 22) & 0x3ff;
    return page_directory + pde_index;
}

u32* memory_virtual_to_pte(uintptr_t virtual_memory)
{
    u16 pte_index = (virtual_memory >> 12) & 0x3ff;
    u16 pde_index = (virtual_memory >> 22) & 0x3ff;
    u32 *page_table_address = (u32*) (page_directory[pde_index] & ~0xfff);
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

void paging_make_page_present(uintptr_t virtual_address)
{
    u32 *pde = memory_virtual_to_pde(virtual_address);
    if (!(*pde & PAGE_FLAG_PRESENT))
    {
        u32 *page_table = (u32*) kernel_memory_allocate(1024 * sizeof(u32), 0x1000);
        memory_set(page_table, 0, 1024 * sizeof(u32));
        *pde = (u32) page_table | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    }
    u32 *pte = memory_virtual_to_pte(virtual_address);
    if (*pte & ~0xfff)
        *pte |= (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    else
        *pte = (u32) pfa_allocate_page() | (PAGE_FLAG_READ_WRITE | PAGE_FLAG_PRESENT);
    page_entry_t page_entry = { virtual_address };
    page_queue_offer(page_entry);
}

void paging_make_page_not_present(uintptr_t virtual_address)
{
    u32 *pte = memory_virtual_to_pte(virtual_address);
    *pte &= ~PAGE_FLAG_PRESENT;
    page_queue_poll();
}

void paging_increment_page_fault_counter()
{
    page_fault_counter++;
}

void paging_reset_page_fault_counter()
{
    page_fault_counter = 0;
}

u32 paging_get_page_fault_count()
{
    return page_fault_counter;
}
