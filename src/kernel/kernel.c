#include <kernel.h>
#include <terminal.h>
#include <io.h>
#include <pfa.h>
#include <idt.h>
#include <paging.h>
#include <page_queue.h>
#include <pfh_fifo.h>
#include <pfh_second.h>

void test_sort();

#define QUEUE_MAX_CAPACITY 1024
#define HEAP_START 0x200000     // Must be page-aligned (4 KB)
#define PAGE_DIRECTORY_START HEAP_START

static const uint16_t PAGES_LIMIT = 6;

static page_entry_t main_queue_memory[QUEUE_MAX_CAPACITY];

__attribute__((unused))
void kernel_main()
{
    terminal_initialize();

    uint32_t identity_pages_count = 1024 * 4;
    void *pages_start_address = (void*) (identity_pages_count * 0x1000);
    void *swap_pages_start_address = pages_start_address - (pfa_get_swap_page_count() * 0x1000);
    pfa_init(pages_start_address, swap_pages_start_address, PAGES_LIMIT);

    paging_init((void*) PAGE_DIRECTORY_START, identity_pages_count);

    idt_set_descriptor(14, &pfh_fifo_isr, 0x8e);
    idt_init();

    page_queue_init(main_queue_memory, PAGES_LIMIT);
    paging_enable((void*) PAGE_DIRECTORY_START);

    test_sort();

    terminal_printf("\nPage Faults: %d\n", paging_get_page_fault_count());
}
