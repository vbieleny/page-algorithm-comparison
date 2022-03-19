#pragma once

#include <types.h>
#include <stddef.h>
#include <stdbool.h>
#include <attrs.h>

typedef struct
{
    uintptr_t virtual_address;
} page_entry_t;

NO_CALLER_SAVED_REGISTERS bool page_queue_offer(page_entry_t page);
NO_CALLER_SAVED_REGISTERS page_entry_t page_queue_poll();
NO_CALLER_SAVED_REGISTERS page_entry_t* page_queue_peek();
NO_CALLER_SAVED_REGISTERS void page_queue_clear();
NO_CALLER_SAVED_REGISTERS void page_queue_set_capacity(size_t capacity);
NO_CALLER_SAVED_REGISTERS size_t page_queue_get_size();
NO_CALLER_SAVED_REGISTERS bool page_queue_is_full();
