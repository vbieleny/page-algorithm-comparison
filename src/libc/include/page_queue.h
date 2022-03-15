#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    uintptr_t virtual_address;
} page_entry_t;

void page_queue_init(void *memory, size_t capacity);
bool page_queue_offer(page_entry_t page);
page_entry_t page_queue_poll();
__attribute__((no_caller_saved_registers)) page_entry_t* page_queue_peek();
size_t page_queue_get_size();
bool page_queue_is_full();
void page_queue_print();
