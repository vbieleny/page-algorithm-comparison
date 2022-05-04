#include <kernel.h>
#include <terminal.h>
#include <io.h>
#include <pfa.h>
#include <idt.h>
#include <paging.h>
#include <pfh_fifo.h>
#include <pfh_second.h>
#include <pra.h>
#include <test_runner.h>
#include <serial.h>
#include <ksystem.h>
#include <kmalloc.h>
#include <malloc.h>
#include <mmap.h>
#include <pic.h>
#include <timer.h>
#include <stdint.h>

#define HEAP_START 0x200000
#define HEAP_SIZE ((IDENTITY_PAGES_COUNT) * 4096 - (HEAP_START))
#define PAGES_START_ADDRESS ((void*) ((IDENTITY_PAGES_COUNT) * 0x1000))

extern const uint32_t KERNEL_START;
extern const uint32_t KERNEL_END;

extern void setup();
extern void asm_kernel_panic_handler();
extern void asm_page_fault_handler();

void kernel_main()
{
    kernel_memory_initialize((void*) HEAP_START, HEAP_SIZE);
    user_memory_initialize();
    terminal_initialize();
    if (!serial_initialize())
    {
        io_sprintf(IO_TERMINAL, "Cannot initialize serial port!\n");
        halt();
    }

    register_page_replacement_algorithm(pra_fifo, "FIFO", &pfh_fifo_isr);
    register_page_replacement_algorithm(pra_second_chance, "Second Chance", &pfh_second_isr);

    pfa_initialize(PAGES_START_ADDRESS);
    paging_initialize(IDENTITY_PAGES_COUNT);

    idt_set_descriptor(13, &asm_kernel_panic_handler, 0x8e);
    idt_set_descriptor(14, &asm_page_fault_handler, 0x8e);
    idt_initialize();

    pic_remap(0x20, 0x28);
    irq_set_mask_all();

    interrupts_enable();
    paging_enable();

    setup();

    if (DEFAULT_STREAM == IO_SERIAL)
        io_shutdown();
}
