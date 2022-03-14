#include <kernel.h>
#include <terminal.h>
#include <string.h>
#include <io.h>
#include <lib.h>
#include <random.h>
#include <memory_map.h>
#include <pfa.h>
#include <idt.h>
#include <paging.h>
#include <page_queue.h>

#define PAGES_LIMIT (4)
#define QUEUE_MAX_CAPACITY (1024)

void test_sort();

// Defined in linker script
extern uint32_t kernel_end;

static uint32_t *page_directory = &kernel_end;
static uint32_t *page_tables = &kernel_end + 0x400;

static int page_fault_count = 0;

static page_entry_t main_queue_memory[QUEUE_MAX_CAPACITY];

static uint32_t* memory_virtual_to_pte(uint32_t virtual_memory)
{
    uint16_t pte_index = (virtual_memory >> 12) & 0x3ff;
    uint16_t pde_index = (virtual_memory >> 22) & 0x3ff;

    uint32_t *page_table_address = (uint32_t*) (page_directory[pde_index] & ~0xfff);
    return page_table_address + pte_index;
}

static void make_page_present(uint32_t cr2, void *free_page)
{
    uint32_t *pte = memory_virtual_to_pte(cr2);
    uint32_t swap_address = (*pte) & ~0xfff;
    *pte = (uint32_t) free_page | 3;
    if (swap_address)
    {
        memmove((void*) (cr2 & ~0xfff), (void*) swap_address, 0x1000);
        mark_swap_free((void*) swap_address);
    }
    page_entry_t page_entry = { cr2 };
    page_queue_offer(page_entry);
}

static void make_page_swapped(uint32_t virtual_address)
{
    void *swap_page = allocate_swap_page();
    memmove(swap_page, (void*) (virtual_address & ~0xfff), 0x1000);
    uint32_t *pte = memory_virtual_to_pte(virtual_address);
    *pte = (uint32_t) swap_page | 2;
    page_queue_poll();
}

__attribute__((interrupt))
void page_fault_handler(interrupt_frame_t* frame, unsigned int error_code)
{
    uint32_t cr2 = read_cr2();
    void *free_page = allocate_page();
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
        invalidate_page(victim_virtual);
    }

    invalidate_page(cr2);
    page_fault_count++;
}

void kernel_main()
{
    terminal_initialize();

    print_memory_map();

    uint32_t identity_pages_count = 1024 * 4;
    init_pages((void*) (identity_pages_count * 0x1000), memory_map, PAGES_LIMIT);
    init_swap_pages((void*) ((identity_pages_count * 0x1000) - (MAX_SWAP_PAGE_COUNT * 0x1000)), memory_map);

    for (int i = 0; i < 1024; i++)
        page_directory[i] = (((uint32_t) page_tables) + (i * 0x1000)) | 3;
    for (int i = 0; i < 1024 * 4; i++)
        page_tables[i] = (i * 0x1000) | 3;

    idt_set_descriptor(14, &page_fault_handler, 0x8e);
    idt_init();

    page_queue_init(main_queue_memory, PAGES_LIMIT);

    enable_paging(page_directory);

    test_sort();

    terminal_printf("\nPage Faults: %d\n", page_fault_count);
}
