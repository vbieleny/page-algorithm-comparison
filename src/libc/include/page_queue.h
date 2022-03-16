#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    uintptr_t virtual_address;
} page_entry_t;

void page_queue_init(void *memory, size_t capacity);
__attribute__((no_caller_saved_registers)) bool page_queue_offer(page_entry_t page);
__attribute__((no_caller_saved_registers)) page_entry_t page_queue_poll();
__attribute__((no_caller_saved_registers)) page_entry_t* page_queue_peek();
__attribute__((no_caller_saved_registers)) void page_queue_clear();
__attribute__((no_caller_saved_registers)) void page_queue_set_capacity(size_t capacity);
__attribute__((no_caller_saved_registers)) size_t page_queue_get_size();
__attribute__((no_caller_saved_registers)) bool page_queue_is_full();
void page_queue_print();
