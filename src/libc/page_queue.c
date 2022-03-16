#include <page_queue.h>
#include <terminal.h>

static page_entry_t *page_queue_memory;
static size_t page_queue_capacity;
static size_t page_queue_size;
static page_entry_t invalid_entry = { 0 };

void page_queue_init(void *memory, size_t capacity)
{
    page_queue_memory = (page_entry_t*) memory;
    page_queue_capacity = capacity;
    page_queue_size = 0;
}

bool page_queue_offer(page_entry_t page)
{
    if (page_queue_size >= page_queue_capacity)
        return false;
    page_queue_memory[page_queue_size++] = page;
    return true;
}

page_entry_t page_queue_poll()
{
    if (page_queue_size <= 0)
        return invalid_entry;
    page_entry_t page_entry = page_queue_memory[0];
    for (int i = 1; i < page_queue_size; i++)
        page_queue_memory[i - 1] = page_queue_memory[i];
    page_queue_size--;
    return page_entry;
}

page_entry_t* page_queue_peek()
{
    if (page_queue_size <= 0)
        return NULL;
    return &page_queue_memory[0];
}

void page_queue_clear()
{
    for (size_t i = 0; i < page_queue_capacity; i++)
        page_queue_memory[i] = invalid_entry;
    page_queue_size = 0;
}

void page_queue_set_capacity(size_t capacity)
{
    page_queue_capacity = capacity;
}

size_t page_queue_get_size()
{
    return page_queue_size;
}

bool page_queue_is_full()
{
    return page_queue_size >= page_queue_capacity;
}

void page_queue_print()
{
    terminal_printf("Size/Capacity: %d/%d\n", page_queue_size, page_queue_capacity);
    page_entry_t *peek = page_queue_peek();
    if (peek)
        terminal_printf("Peek: 0x%x\n", peek->virtual_address);
    else
        terminal_printf("Peek: NULL\n");
    terminal_printf("Contents: [");
    for (int i = 0; i < page_queue_size; i++)
    {
        const char *format = i == page_queue_size - 1 ? "0x%x" : "0x%x, ";
        terminal_printf(format, page_queue_memory[i].virtual_address);
    }
    terminal_printf("]\n");
}
