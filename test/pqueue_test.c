#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "pra_unit.h"
#include "pra_pqueue.h"
#include "pra_kmalloc.h"

static const size_t DEFAULT_PAGE_QUEUE_CAPACITY = 4;

static bool test_pqueue_initial_size()
{
    PRA_ASSERT(page_queue_get_size() == 0);
    return true;
}

static bool test_pqueue_clear()
{
    page_queue_clear();
    PRA_ASSERT(page_queue_get_size() == 0);
    return true;
}

static bool test_pqueue_offer()
{
    page_queue_clear();

    page_entry_t page = { 50 };
    PRA_ASSERT(page_queue_offer(page));
    PRA_ASSERT(page_queue_get_size() == 1);
    return true;
}

static bool test_pqueue_peek()
{
    page_queue_clear();

    page_entry_t page = { 50 };
    PRA_ASSERT(page_queue_offer(page));
    PRA_ASSERT(page_queue_get_size() == 1);
    
    page_entry_t *peeked_page = page_queue_peek();
    PRA_ASSERT(peeked_page->virtual_address == page.virtual_address);
    return true;
}

static bool test_pqueue_poll()
{
    page_queue_clear();

    page_entry_t first_page = { 20 };
    PRA_ASSERT(page_queue_offer(first_page));
    PRA_ASSERT(page_queue_get_size() == 1);

    page_entry_t second_page = { 25 };
    PRA_ASSERT(page_queue_offer(second_page));
    PRA_ASSERT(page_queue_get_size() == 2);
    
    page_entry_t polled_page = page_queue_poll();
    PRA_ASSERT(polled_page.virtual_address == first_page.virtual_address);
    PRA_ASSERT(page_queue_get_size() == 1);
    return true;
}

static bool test_pqueue_full()
{
    page_queue_clear();

    for (size_t i = 0; i < DEFAULT_PAGE_QUEUE_CAPACITY; i++)
    {
        page_entry_t page = { (i + 1) * 10 };
        PRA_ASSERT(page_queue_offer(page));
        PRA_ASSERT(page_queue_get_size() == i + 1);
        PRA_ASSERT(i == DEFAULT_PAGE_QUEUE_CAPACITY - 1 ? page_queue_is_full() : !page_queue_is_full());
    }

    return true;
}

static bool test_pqueue_send_to_back()
{
    page_queue_clear();

    page_entry_t first_page = { 12 };
    PRA_ASSERT(page_queue_offer(first_page));
    PRA_ASSERT(page_queue_get_size() == 1);

    page_entry_t second_page = { 22 };
    PRA_ASSERT(page_queue_offer(second_page));
    PRA_ASSERT(page_queue_get_size() == 2);

    page_entry_t third_page = { 44 };
    PRA_ASSERT(page_queue_offer(third_page));
    PRA_ASSERT(page_queue_get_size() == 3);

    page_entry_t *peeked_page = page_queue_peek();
    PRA_ASSERT(peeked_page->virtual_address == first_page.virtual_address);

    page_queue_send_to_back();

    peeked_page = page_queue_peek();
    PRA_ASSERT(peeked_page->virtual_address == second_page.virtual_address);

    page_queue_send_to_back();

    peeked_page = page_queue_peek();
    PRA_ASSERT(peeked_page->virtual_address == third_page.virtual_address);

    page_queue_send_to_back();

    peeked_page = page_queue_peek();
    PRA_ASSERT(peeked_page->virtual_address == first_page.virtual_address);
    
    return true;
}

static bool test_pqueue_combined()
{
    page_queue_clear();

    page_entry_t first_page = { 12 };
    PRA_ASSERT(page_queue_offer(first_page));
    PRA_ASSERT(page_queue_get_size() == 1);

    page_entry_t second_page = { 22 };
    PRA_ASSERT(page_queue_offer(second_page));
    PRA_ASSERT(page_queue_get_size() == 2);

    page_entry_t *peeked_page = page_queue_peek();
    PRA_ASSERT(peeked_page->virtual_address == first_page.virtual_address);

    page_entry_t third_page = { 44 };
    PRA_ASSERT(page_queue_offer(third_page));
    PRA_ASSERT(page_queue_get_size() == 3);

    page_entry_t polled_page = page_queue_poll();
    PRA_ASSERT(page_queue_get_size() == 2);
    PRA_ASSERT(polled_page.virtual_address == first_page.virtual_address);

    peeked_page = page_queue_peek();
    PRA_ASSERT(peeked_page->virtual_address == second_page.virtual_address);

    page_queue_send_to_back();

    peeked_page = page_queue_peek();
    PRA_ASSERT(page_queue_get_size() == 2);
    PRA_ASSERT(peeked_page->virtual_address == third_page.virtual_address);

    PRA_ASSERT(page_queue_offer(first_page));
    PRA_ASSERT(page_queue_offer(third_page));

    PRA_ASSERT(page_queue_get_size() == 4);
    PRA_ASSERT(page_queue_is_full());
    
    return true;
}

static bool test_pqueue_grow_capacity()
{
    page_queue_clear();

    for (size_t i = 0; i < DEFAULT_PAGE_QUEUE_CAPACITY; i++)
    {
        page_entry_t page = { (i + 1) * 10 };
        PRA_ASSERT(page_queue_offer(page));
        PRA_ASSERT(page_queue_get_size() == i + 1);
        PRA_ASSERT(i == DEFAULT_PAGE_QUEUE_CAPACITY - 1 ? page_queue_is_full() : !page_queue_is_full());
    }

    PRA_ASSERT(page_queue_is_full());
    page_queue_set_capacity(DEFAULT_PAGE_QUEUE_CAPACITY + 8);
    PRA_ASSERT(!page_queue_is_full());

    page_queue_set_capacity(DEFAULT_PAGE_QUEUE_CAPACITY);

    return true;
}

static bool test_pqueue_shrink_capacity()
{
    page_queue_clear();

    page_entry_t first_page = { 100 };
    PRA_ASSERT(page_queue_offer(first_page));
    PRA_ASSERT(page_queue_get_size() == 1);

    page_entry_t second_page = { 200 };
    PRA_ASSERT(page_queue_offer(second_page));
    PRA_ASSERT(page_queue_get_size() == 2);

    PRA_ASSERT(!page_queue_is_full());
    page_queue_set_capacity(2);
    PRA_ASSERT(page_queue_is_full());

    page_queue_set_capacity(DEFAULT_PAGE_QUEUE_CAPACITY);

    return true;
}

static bool all_tests()
{
    PRA_RUN_TEST(test_pqueue_initial_size);
    PRA_RUN_TEST(test_pqueue_clear);
    PRA_RUN_TEST(test_pqueue_offer);
    PRA_RUN_TEST(test_pqueue_peek);
    PRA_RUN_TEST(test_pqueue_poll);
    PRA_RUN_TEST(test_pqueue_full);
    PRA_RUN_TEST(test_pqueue_send_to_back);
    PRA_RUN_TEST(test_pqueue_combined);
    PRA_RUN_TEST(test_pqueue_grow_capacity);
    PRA_RUN_TEST(test_pqueue_shrink_capacity);
    return true;
}

int main(int argc, char *argv[])
{
    size_t memory_size = 1024 * 16;
    uint8_t *memory_start = malloc(memory_size);
    kernel_memory_initialize(memory_start, memory_size);
    page_queue_set_capacity(DEFAULT_PAGE_QUEUE_CAPACITY);

    all_tests();

    free(memory_start);
    
    return 0;
}
