#include <kernel.h>
#include <terminal.h>
#include <io.h>
#include <pfa.h>
#include <idt.h>
#include <paging.h>
#include <pfh_fifo.h>
#include <pfh_second.h>
#include <test_runner.h>
#include <serial.h>
#include <ksystem.h>
#include <kmalloc.h>
#include <malloc.h>
#include <memory_map.h>
#include <types.h>

void test_sort();

#define IDENTITY_PAGES_COUNT 4096
#define HEAP_START 0x200000
#define HEAP_SIZE ((IDENTITY_PAGES_COUNT) * 4096 - (HEAP_START))
#define PAGES_START_ADDRESS ((void*) ((IDENTITY_PAGES_COUNT) * 0x1000))

extern const u32 KERNEL_START;
extern const u32 KERNEL_END;

void kernel_main()
{
    kernel_memory_init((void*) HEAP_START, HEAP_SIZE);
    memory_init();
    terminal_initialize();
    if (!serial_init())
    {
        io_printf(DEFAULT_STREAM, "Cannot initialize serial port!\n");
        halt();
    }

    u32 kernel_size = ((u32) (&KERNEL_END)) - ((u32) (&KERNEL_START));
    io_printf(DEFAULT_STREAM, "Kernel Size: %d KB\n\n", kernel_size / 1024);

    memory_map_print();
    io_printf(DEFAULT_STREAM, "\n");

    pfa_init(PAGES_START_ADDRESS, 16 * 1024);
    paging_init(IDENTITY_PAGES_COUNT);

    idt_set_descriptor(13, &kernel_panic, 0x8e);
    idt_init();

    paging_enable();

    run_test("Linked List Sort", "FIFO", &pfh_fifo_isr, &test_sort, 2, 8);
    run_test("Linked List Sort", "Second Chance", &pfh_second_isr, &test_sort, 2, 8);

    run_test("Linked List Sort", "FIFO", &pfh_fifo_isr, &test_sort, 4, 8);
    run_test("Linked List Sort", "Second Chance", &pfh_second_isr, &test_sort, 4, 8);

    run_test("Linked List Sort", "FIFO", &pfh_fifo_isr, &test_sort, 6, 8);
    run_test("Linked List Sort", "Second Chance", &pfh_second_isr, &test_sort, 6, 8);
}
