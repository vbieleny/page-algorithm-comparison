#include <kernel.h>
#include <terminal.h>
#include <io.h>
#include <lib.h>
#include <pfa.h>
#include <idt.h>
#include <paging.h>
#include <page_queue.h>

void test_sort();

#define QUEUE_MAX_CAPACITY 1024
#define HEAP_START 0x200000     // Must be page-aligned (4 KB)
#define PAGE_DIRECTORY_START HEAP_START

static const uint16_t PAGES_LIMIT = 4;

static page_entry_t main_queue_memory[QUEUE_MAX_CAPACITY];
static int page_fault_count = 0;

__attribute__((no_caller_saved_registers))
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

__attribute__((no_caller_saved_registers))
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
    uint32_t accessed_address = paging_read_cr2();
    void *free_page = pfa_page_allocate();
    if (free_page)
    {
        make_page_present(accessed_address, free_page);
    }
    else
    {
        uint32_t victim_virtual = page_queue_peek()->virtual_address;
        uint32_t *victim_pte = memory_virtual_to_pte(victim_virtual);
        void *victim_address = (void*) ((*victim_pte) & ~0xfff);
        make_page_swapped(victim_virtual);
        make_page_present(accessed_address, victim_address);
        paging_invalidate_page(victim_virtual);
    }

    paging_invalidate_page(accessed_address);
    page_fault_count++;
}

__attribute__((unused))
void kernel_main()
{
    terminal_initialize();

    uint32_t identity_pages_count = 1024 * 4;
    void *pages_start_address = (void*) (identity_pages_count * 0x1000);
    void *swap_pages_start_address = pages_start_address - (pfa_get_swap_page_count() * 0x1000);
    pfa_init(pages_start_address, swap_pages_start_address, PAGES_LIMIT);

    paging_init((void*) PAGE_DIRECTORY_START, identity_pages_count);

    idt_set_descriptor(14, &page_fault_handler, 0x8e);
    idt_init();

    page_queue_init(main_queue_memory, PAGES_LIMIT);
    paging_enable((void*) PAGE_DIRECTORY_START);

    test_sort();

    terminal_printf("\nPage Faults: %d\n", page_fault_count);
}
