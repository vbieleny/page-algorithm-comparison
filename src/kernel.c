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

    uint32_t kernel_size = ((uint32_t) (&KERNEL_END)) - ((uint32_t) (&KERNEL_START));
    io_printf("Kernel Size: %d KB\n\n", kernel_size / 1024);

    memory_map_print();
    io_printf("\n");

    pfa_initialize(PAGES_START_ADDRESS);
    paging_initialize(IDENTITY_PAGES_COUNT);
    timer_initialize();

    idt_set_descriptor(13, &kernel_panic, 0x8e);
    idt_initialize();

    pic_remap(0x20, 0x28);
    irq_set_mask_all();
    irq_clear_mask(IRQ_TIMER);

    timer_set_divisor(TIMER_DIVISOR_991_HZ);

    interrupts_enable();

    paging_enable();

    setup();
}
