#pragma once

#include <idt.h>
#include <attrs.h>
#include <paging.h>

void pfh_random_init();
void pfh_random_destroy();
void pfh_random_isr(uint32_t error_code, page_fault_handler_result_t *result);
