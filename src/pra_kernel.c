#include "pra_kernel.h"
#include "pra_terminal.h"
#include "pra_io.h"
#include "pra_pfa.h"
#include "pra_idt.h"
#include "pra_paging.h"
#include "pra_pfh_fifo.h"
#include "pra_pfh_second.h"
#include "pra_pfh_random.h"
#include "pra_pfh.h"
#include "pra_suite.h"
#include "pra_serial.h"
#include "pra_system.h"
#include "pra_kmalloc.h"
#include "pra_malloc.h"
#include "pra_mmap.h"
#include "pra_pic.h"
#include "pra_timer.h"
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

    register_page_replacement_algorithm(pra_fifo, "FIFO", NULL, NULL, &pfh_fifo_isr);
    register_page_replacement_algorithm(pra_second_chance, "Second Chance", NULL, NULL, &pfh_second_isr);
    register_page_replacement_algorithm(pra_random, "Random", &pfh_random_init, &pfh_random_destroy, &pfh_random_isr);

    pfa_initialize(PAGES_START_ADDRESS);
    paging_initialize(IDENTITY_PAGES_COUNT);

    idt_set_descriptor(13, &asm_kernel_panic_handler, 0x8e);
    idt_set_descriptor(14, &asm_page_fault_handler, 0x8e);
    idt_initialize();

    pic_remap(0x20, 0x28);
    irq_set_mask_all();

    set_interrupts_enabled(true);
    paging_enable();

    setup();

    io_printf("All tests executed!\n");

    if (DEFAULT_STREAM == IO_SERIAL)
        io_shutdown();
}
