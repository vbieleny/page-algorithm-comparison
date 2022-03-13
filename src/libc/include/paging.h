#pragma once

#include <stdint.h>

void enable_paging(uint32_t *page_directory);
void invalidate_page(uint32_t address);
uint32_t read_cr2();
