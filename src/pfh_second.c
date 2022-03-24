#include <pfh_second.h>
#include <pfa.h>
#include <paging.h>
#include <pqueue.h>

NO_CALLER_SAVED_REGISTERS static void send_page_to_back()
{
    page_queue_send_to_back();
}

void pfh_second_isr(interrupt_frame_t* frame, uint32_t error_code)
{
    uint32_t accessed_address = paging_get_accessed_address();
    if (!pfa_is_page_allocation_limit_reached())
    {
        paging_make_page_present(accessed_address);
    }
    else
    {
        uint32_t victim_virtual;
        uint32_t *victim_pte;
        for (;;)
        {
            victim_virtual = page_queue_peek()->virtual_address;
            victim_pte = memory_virtual_to_pte(victim_virtual);
            if (!(*victim_pte & PAGE_FLAG_ACCESSED))
                break;
            *victim_pte &= ~PAGE_FLAG_ACCESSED;
            paging_invalidate_page(victim_virtual);
            send_page_to_back();
        }
        paging_make_page_not_present(victim_virtual);
        paging_make_page_present(accessed_address);
        paging_invalidate_page(victim_virtual);
    }

    paging_invalidate_page(accessed_address);
    paging_increment_page_fault_counter();
}