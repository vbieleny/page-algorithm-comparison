#include <pfh_fifo.h>
#include <pfa.h>
#include <paging.h>
#include <page_queue.h>

void pfh_fifo_isr(interrupt_frame_t* frame, unsigned int error_code)
{
    uint32_t accessed_address = paging_read_cr2();
    void *free_page = pfa_page_allocate();
    if (free_page)
    {
        paging_make_page_present(accessed_address, free_page);
    }
    else
    {
        uint32_t victim_virtual = page_queue_peek()->virtual_address;
        uint32_t *victim_pte = memory_virtual_to_pte(victim_virtual);
        void *victim_address = (void*) ((*victim_pte) & ~0xfff);
        paging_make_page_swapped(victim_virtual);
        paging_make_page_present(accessed_address, victim_address);
        paging_invalidate_page(victim_virtual);
    }

    paging_invalidate_page(accessed_address);
    paging_increment_page_fault_counter();
}
