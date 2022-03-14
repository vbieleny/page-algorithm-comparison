#include <kernel.h>
#include <terminal.h>
#include <string.h>
#include <io.h>
#include <lib.h>
#include <memory_map.h>
#include <pfa.h>
#include <idt.h>
#include <paging.h>

// Defined in linker script
extern uint32_t kernel_end;

static uint32_t *page_directory = &kernel_end;
static uint32_t *page_tables = &kernel_end + 0x400;

static int page_fault_count = 0;

__attribute__((interrupt))
void page_fault_handler(interrupt_frame_t* frame, unsigned int error_code)
{
    uint32_t cr2 = read_cr2();
    uint16_t pte_index = (cr2 >> 12) & 0x3ff;
    uint16_t pde_index = (cr2 >> 22) & 0x3ff;

    uint32_t *page_table_address = (uint32_t*) (page_directory[pde_index] & ~0xfff);
    page_table_address[pte_index] |= (cr2 & ~0xfff) | 3;

    invalidate_page(cr2);

    page_fault_count++;
}

void kernel_main()
{
    terminal_initialize();

    print_memory_map();

    uint8_t *pages_start_address = (uint8_t*) &kernel_end;
    pages_start_address += 1024 * 4;            // Page Directory
    pages_start_address += 1024 * 1024 * 4;     // Page Tables
    init_pages(pages_start_address, memory_map, 4);

    for (int i = 0; i < 1024; i++)
        page_directory[i] = (((uint32_t) page_tables) + (i * 0x1000)) | 3;
    for (int i = 0; i < 1024 * 4; i++)
        page_tables[i] = (i * 0x1000) | 3;

    idt_set_descriptor(14, &page_fault_handler, 0x8e);
    idt_init();

    enable_paging(page_directory);

    terminal_printf("Page Faults: %d\n", page_fault_count);
}
