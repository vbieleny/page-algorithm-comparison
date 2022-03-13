#include <kernel.h>
#include <io.h>
#include <terminal.h>
#include <idt.h>
#include <paging.h>

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
    idt_set_descriptor(14, &page_fault_handler, 0x8e);
    idt_init();

    for (int i = 0; i < 1024; i++)
        page_directory[i] = (((uint32_t) page_tables) + (i * 0x1000)) | 3;
    for (int i = 0; i < 1024 * 4; i++)
        page_tables[i] = (i * 0x1000) | 3;
    enable_paging(page_directory);

    char *address = (char*) 0x01000000;
    terminal_printf("%x\n", *address);
    address = (char*) 0x02000000;
    terminal_printf("%x\n", *address);
    address = (char*) 0x03000000;
    terminal_printf("%x\n", *address);
    address = (char*) 0x04000000;
    terminal_printf("%x\n", *address);

    terminal_printf("Hello, World\n");
    terminal_printf("Page Faults: %d", page_fault_count);
}
