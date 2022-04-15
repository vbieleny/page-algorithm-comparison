#include <test_runner.h>
#include <idt.h>
#include <pfa.h>
#include <pqueue.h>
#include <malloc.h>
#include <paging.h>
#include <random.h>
#include <io.h>

uint32_t run_test(const char *test_name, const char *isr_name, page_replacement_algorithm_e pra, void (*test_function)(), uint32_t pages_limit, uint32_t allocation_spread)
{
    idt_set_descriptor(14, get_page_replacement_function(pra), 0x8e);

    pfa_set_max_pages(pages_limit);
    pfa_free_all_pages();

    paging_reset_to_default();

    page_queue_clear();
    page_queue_set_capacity(pages_limit);

    user_memory_set_memory_size(allocation_spread);
    user_memory_free_all();

    srand(1);

    io_printf("%s | %s | Pages: %d | Spread: %d (%d KB)\n", test_name, isr_name, pages_limit, allocation_spread, (allocation_spread * 0x1000) / 1024);
    test_function();
    uint32_t page_faults = paging_get_page_fault_count();
    io_printf("Page Faults: %d\n\n", page_faults);
    return page_faults;
}
