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
#include <kmalloc.h>

void test_sort();

#define IDENTITY_PAGES_COUNT 4096           // First 16 MB (4096 pages with 4 KB each)
#define HEAP_START 0x200000                 // 2 MB
#define HEAP_SIZE (0x1000000 - 0x200000)    // 14 MB (from 2 MB to 16 MB)

extern const uint32_t KERNEL_END;

__attribute__((noreturn))
__attribute__((interrupt))
static void kernel_panic(interrupt_frame_t* frame, unsigned int error_code);

__attribute__((noreturn))
__attribute__((no_caller_saved_registers))
static void halt();

__attribute__((unused))
void kernel_main()
{
    kernel_memory_init((void*) HEAP_START, HEAP_SIZE);
    terminal_initialize();
    if (!serial_init())
    {
        io_printf(DEFAULT_STREAM, "Cannot initialize serial port!\n");
        halt();
    }

    uint32_t kernel_size = ((uint32_t) (&KERNEL_END)) - 0x100000;
    io_printf(DEFAULT_STREAM, "Kernel Size: %d KB\n\n", kernel_size / 1024);

    void *pages_start_address = (void*) (IDENTITY_PAGES_COUNT * 0x1000);
    void *swap_pages_start_address = pages_start_address - (pfa_get_swap_page_count() * 0x1000);
    pfa_init(pages_start_address, swap_pages_start_address);
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

static void kernel_panic(interrupt_frame_t* frame, unsigned int error_code)
{
    io_printf(DEFAULT_STREAM, "Kernel Panic (Error Code: 0x%x)\n", error_code);
    halt();
}

static void halt()
{
    for (;;)
        asm volatile("hlt");
}
