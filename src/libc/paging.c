#include <paging.h>

void paging_enable(uint32_t *page_directory);
void paging_invalidate_page(uintptr_t address);
uintptr_t paging_read_cr2();
