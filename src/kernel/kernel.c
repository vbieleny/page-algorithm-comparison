#include <kernel.h>
#include <terminal.h>
#include <string_utils.h>
#include <io.h>
#include <lib.h>
#include <random.h>
#include <memory_map.h>
#include <pfa.h>
#include <idt.h>
#include <paging.h>
#include <page_queue.h>
#include <kernel_malloc.h>

void test_sort();

#define QUEUE_MAX_CAPACITY 1024

static const uintptr_t HEAP_START = 0x200000;
static const uint16_t PAGES_LIMIT = 4;
extern const uint32_t KERNEL_END;

static uint32_t *page_directory = (uint32_t*) 0x200000;
static uint32_t *page_tables = (uint32_t*) (0x200000 + 0x1000);
static page_entry_t main_queue_memory[QUEUE_MAX_CAPACITY];
static int page_fault_count = 0;

static uint32_t* memory_virtual_to_pte(uintptr_t virtual_memory)
{
    uint16_t pte_index = (virtual_memory >> 12) & 0x3ff;
    uint16_t pde_index = (virtual_memory >> 22) & 0x3ff;

    uint32_t *page_table_address = (uint32_t*) (page_directory[pde_index] & ~0xfff);
    return page_table_address + pte_index;
}

static void make_page_present(uintptr_t cr2, void *free_page)
{
    uint32_t *pte = memory_virtual_to_pte(cr2);
    uint32_t swap_address = (*pte) & ~0xfff;
    *pte = (uint32_t) free_page | 3;
    if (swap_address)
    {
        memory_move((void*) (cr2 & ~0xfff), (void*) swap_address, 0x1000);
        pfa_mark_swap_free((void*) swap_address);
    }
    page_entry_t page_entry = { cr2 };
    page_queue_offer(page_entry);
}

static void make_page_swapped(uintptr_t virtual_address)
{
    void *swap_page = pfa_swap_page_allocate();
    memory_move(swap_page, (void*) (virtual_address & ~0xfff), 0x1000);
    uint32_t *pte = memory_virtual_to_pte(virtual_address);
    *pte = (uint32_t) swap_page | 2;
    page_queue_poll();
}

__attribute__((interrupt))
void page_fault_handler(interrupt_frame_t* frame, unsigned int error_code)
{
    uint32_t cr2 = paging_read_cr2();
    void *free_page = pfa_page_allocate();
    if (free_page)
    {
        make_page_present(cr2, free_page);
    }
    else
    {
        uint32_t victim_virtual = page_queue_peek()->virtual_address;
        uint32_t *victim_pte = memory_virtual_to_pte(victim_virtual);
        void *free_address = (void*) ((*victim_pte) & ~0xfff);
        make_page_swapped(victim_virtual);
        make_page_present(cr2, free_address);
        paging_invalidate_page(victim_virtual);
    }

    paging_invalidate_page(cr2);
    page_fault_count++;
}

void kernel_main()
{
    kernel_set_heap_start((void*) HEAP_START);
    terminal_initialize();

    uint32_t identity_pages_count = 1024 * 4;
    void *pages_start_address = (void*) (identity_pages_count * 0x1000);
    void *swap_pages_start_address = pages_start_address - (pfa_get_swap_page_count() * 0x1000);
    pfa_init(pages_start_address, swap_pages_start_address, PAGES_LIMIT);

    for (size_t i = 0; i < 1024; i++)
        page_directory[i] = (((uint32_t) page_tables) + (i * 0x1000)) | 3;
    for (size_t i = 0; i < identity_pages_count; i++)
        page_tables[i] = (i * 0x1000) | 3;

    idt_set_descriptor(14, &page_fault_handler, 0x8e);
    idt_init();

    page_queue_init(main_queue_memory, PAGES_LIMIT);

    paging_enable(page_directory);

    test_sort();

    terminal_printf("\nPage Faults: %d\n", page_fault_count);
}
