#include "pra_attrs.h"
#include "pra_pfh_second.h"
#include "pra_pfa.h"
#include "pra_paging.h"
#include "pra_pqueue.h"
#include "pra_lib.h"

void pfh_second_isr(uint32_t error_code, page_fault_handler_result_t *result)
{
    uint32_t accessed_address = paging_get_accessed_address();
    if (!pfa_is_page_allocation_limit_reached())
    {
        paging_make_page_present(accessed_address);
        page_entry_t page_entry = { accessed_address };
        page_queue_offer(page_entry);
    }
    else
    {
        uint32_t victim_virtual;
        page_table_entry_t *victim_pte;
        for (;;)
        {
            victim_virtual = page_queue_peek()->virtual_address;
            victim_pte = (page_table_entry_t*) memory_virtual_to_pte(victim_virtual);
            if (!victim_pte->accessed)
                break;
            victim_pte->accessed = 0;
            paging_invalidate_page(victim_virtual);
            page_queue_send_to_back();
        }
        page_fault_handler_result_fill(result, victim_pte);
        victim_pte->dirty = 0;
        paging_make_page_not_present(victim_virtual);
        page_queue_poll();
        paging_make_page_present(accessed_address);
        page_entry_t page_entry = { accessed_address };
        page_queue_offer(page_entry);
        paging_invalidate_page(victim_virtual);
    }

    paging_invalidate_page(accessed_address);
}
