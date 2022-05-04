#pragma once

#include <idt.h>
#include <attrs.h>
#include <paging.h>

void pfh_fifo_isr(uint32_t error_code, page_fault_handler_result_t *result);
