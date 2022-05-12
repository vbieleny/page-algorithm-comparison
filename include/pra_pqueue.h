#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "pra_attrs.h"

typedef struct
{
    uintptr_t virtual_address;
} page_entry_t;

bool page_queue_offer(page_entry_t page);
page_entry_t page_queue_poll();
page_entry_t* page_queue_peek();
void page_queue_send_to_back();
void page_queue_clear();
void page_queue_set_capacity(size_t capacity);
size_t page_queue_get_size();
bool page_queue_is_full();
