#include <pfh_fifo.h>
#include <pfa.h>
#include <paging.h>
#include <page_queue.h>

void pfh_fifo_isr(interrupt_frame_t* frame, u32 error_code)
{
    u32 accessed_address = paging_read_cr2();
    if (!pfa_is_allocation_limit_reached())
    {
        paging_make_page_present(accessed_address);
    }
    else
    {
        u32 victim_virtual = page_queue_peek()->virtual_address;
        paging_make_page_not_present(victim_virtual);
        paging_make_page_present(accessed_address);
        paging_invalidate_page(victim_virtual);
    }

    paging_invalidate_page(accessed_address);
    paging_increment_page_fault_counter();
}
