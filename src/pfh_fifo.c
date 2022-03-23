#include <pfh_fifo.h>
#include <pfa.h>
#include <paging.h>
#include <pqueue.h>
#include <io.h>

void pfh_fifo_isr(interrupt_frame_t* frame, uint32_t error_code)
{
    uint32_t accessed_address = paging_get_accessed_address();
    if (!pfa_is_page_allocation_limit_reached())
    {
        paging_make_page_present(accessed_address);
    }
    else
    {
        uint32_t victim_virtual = page_queue_peek()->virtual_address;
        paging_make_page_not_present(victim_virtual);
        paging_make_page_present(accessed_address);
        paging_invalidate_page(victim_virtual);
    }
    paging_invalidate_page(accessed_address);
    paging_increment_page_fault_counter();
}
