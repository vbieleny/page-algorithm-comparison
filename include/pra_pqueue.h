/**
 * @file
 * @brief A queue implementation to store pages for a page replacement algorithms.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "pra_attrs.h"

/**
 * @brief A structure containing virtual address that caused the page fault.
 */
typedef struct
{
    uintptr_t virtual_address;
} page_entry_t;

/**
 * @brief Offers the page entry element to the back of the queue.
 * @param page the page to insert to the queue
 * @return true if the element was added to the queue, otherwise false
 */
bool page_queue_offer(page_entry_t page);

/**
 * @brief Returns the element at the front of the queue and removes it.
 * @return removed element from the front of the queue
 */
page_entry_t page_queue_poll();

/**
 * @brief Returns the element at the front of the queue without removing it.
 * @return element at the front of the queue
 */
page_entry_t* page_queue_peek();

/**
 * @brief Sends the element at the front of the queue to the back of the queue.
 */
void page_queue_send_to_back();

/**
 * @brief Removes all elements from the queue.
 */
void page_queue_clear();

/**
 * @brief Sets the capacity of the queue. When the queue is full, it won't allow adding new elements.
 * @param capacity the new capacity of the queue
 */
void page_queue_set_capacity(size_t capacity);

/**
 * @brief Returns how many elements are in the queue.
 * @return the size of the queue
 */
size_t page_queue_get_size();

/**
 * @brief Returns whether the queue has reached its capacity and is full.
 * @return true if the queue is full, otherwise false
 */
bool page_queue_is_full();
