#include <test_runner.h>
#include <idt.h>
#include <pfa.h>
#include <page_queue.h>
#include <malloc.h>
#include <paging.h>
#include <random.h>
#include <io.h>

u32 run_test(const char *test_name, const char *isr_name, void *pra_isr, void (*test_function)(), u32 pages_limit, u32 allocation_spread)
{
    idt_set_descriptor(14, pra_isr, 0x8e);
    pfa_set_allocation_limit(pages_limit);
    pfa_free_all();
    paging_reset();
    paging_reset_page_fault_counter();
    page_queue_clear();
    page_queue_set_capacity(pages_limit);
    set_max_pages_to_allocate(allocation_spread);
    memory_free_all();
    random_set_seed(1);

    io_printf(DEFAULT_STREAM, "%s | %s | Pages: %d | Spread: %d (%d KB)\n", test_name, isr_name, pages_limit, allocation_spread, (allocation_spread * 0x1000) / 1024);
    test_function();
    u32 page_faults = paging_get_page_fault_count();
    io_printf(DEFAULT_STREAM, "Page Faults: %d\n\n", page_faults);
    return page_faults;
}
