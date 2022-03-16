#include <kernel.h>
#include <terminal.h>
#include <io.h>
#include <pfa.h>
#include <idt.h>
#include <paging.h>
#include <page_queue.h>
#include <pfh_fifo.h>
#include <pfh_second.h>
#include <test_runner.h>
#include <serial.h>

void test_sort();

#define QUEUE_MAX_CAPACITY 1024
#define HEAP_START 0x200000     // Must be page-aligned (4 KB)
#define PAGE_DIRECTORY_START HEAP_START

extern const uint32_t KERNEL_END;
static page_entry_t main_queue_memory[QUEUE_MAX_CAPACITY];

__attribute__((unused))
void kernel_main()
{
    terminal_initialize();
    if (!serial_init())
    {
        io_printf(DEFAULT_STREAM, "Cannot initialize serial port!\n");
        asm volatile("hlt");
    }

    uint32_t kernel_size = ((uint32_t) (&KERNEL_END)) - 0x100000;
    io_printf(DEFAULT_STREAM, "Kernel Size: %d KB\n", kernel_size / 1024);

    uint32_t identity_pages_count = 1024 * 4;
    void *pages_start_address = (void*) (identity_pages_count * 0x1000);
    void *swap_pages_start_address = pages_start_address - (pfa_get_swap_page_count() * 0x1000);
    pfa_init(pages_start_address, swap_pages_start_address);
    paging_init((void*) PAGE_DIRECTORY_START, identity_pages_count);
    idt_init();
    page_queue_init(main_queue_memory, 0);
    paging_enable((void*) PAGE_DIRECTORY_START);

    run_test("Linked List Sort", "FIFO", &pfh_fifo_isr, &test_sort, 4, 8);
    run_test("Linked List Sort", "Second Chance", &pfh_second_isr, &test_sort, 4, 8);

    run_test("Linked List Sort", "FIFO", &pfh_fifo_isr, &test_sort, 6, 8);
    run_test("Linked List Sort", "Second Chance", &pfh_second_isr, &test_sort, 6, 8);
}
