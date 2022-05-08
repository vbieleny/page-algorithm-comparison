#include <attrs.h>
#include <pfh_second.h>
#include <pfa.h>
#include <paging.h>
#include <pqueue.h>
#include <lib.h>
#include "list.h"
#include "random.h"

static list_t *pages_list;

void pfh_random_init()
{
    pages_list = list_create();
}

void pfh_random_destroy()
{
    list_destroy(pages_list);
    pages_list = NULL;
}

void pfh_random_isr(uint32_t error_code, page_fault_handler_result_t *result)
{
    uint32_t accessed_address = paging_get_accessed_address();
    if (!pfa_is_page_allocation_limit_reached())
    {
        paging_make_page_present(accessed_address);
        list_append(pages_list, (void*) accessed_address);
    }
    else
    {
        uint32_t random_page_index = pra_rand_between(0, list_get_size(pages_list) - 1);
        uint32_t victim_virtual = (uint32_t) list_get(pages_list, random_page_index);
        list_set(pages_list, random_page_index, (void*) accessed_address);
        page_table_entry_t *victim_pte = (page_table_entry_t*) memory_virtual_to_pte(victim_virtual);
        page_fault_handler_result_fill(result, victim_pte);
        victim_pte->dirty = 0;
        paging_make_page_not_present(victim_virtual);
        paging_make_page_present(accessed_address);
        paging_invalidate_page(victim_virtual);
    }

    paging_invalidate_page(accessed_address);
}
