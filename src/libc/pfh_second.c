#include <pfh_second.h>
#include <pfa.h>
#include <paging.h>
#include <page_queue.h>

__attribute__((no_caller_saved_registers)) static void send_page_to_back()
{
    page_entry_t victim_page = page_queue_poll();
    page_queue_offer(victim_page);
}

void pfh_second_isr(interrupt_frame_t* frame, unsigned int error_code)
{
    uint32_t accessed_address = paging_read_cr2();
    void *free_page = pfa_page_allocate();
    if (free_page)
    {
        paging_make_page_present(accessed_address, free_page);
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
            *victim_pte = *victim_pte & ~PAGE_FLAG_ACCESSED;
            send_page_to_back();
        }
        void *victim_address = (void*) ((*victim_pte) & ~0xfff);
        paging_make_page_swapped(victim_virtual);
        paging_make_page_present(accessed_address, victim_address);
        paging_invalidate_page(victim_virtual);
    }

    paging_invalidate_page(accessed_address);
    paging_increment_page_fault_counter();
}
