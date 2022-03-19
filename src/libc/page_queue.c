#include <page_queue.h>
#include <io.h>
#include <kmalloc.h>

static page_entry_t *page_queue_memory;
static size_t page_queue_capacity;
static size_t page_queue_size;
static page_entry_t invalid_entry = { 0 };

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
    if (page_queue_capacity != capacity)
    {
        if (!page_queue_memory)
            page_queue_memory = (page_entry_t*) kernel_memory_allocate(capacity * sizeof(page_entry_t), 1);
        else
            page_queue_memory = (page_entry_t*) kernel_memory_reallocate(page_queue_memory, capacity * sizeof(page_entry_t), 1);
    }
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
