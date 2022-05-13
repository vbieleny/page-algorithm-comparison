/**
 * @file
 * @brief Implementation of FIFO page replacement algorithm.
 */

#pragma once

#include "pra_idt.h"
#include "pra_attrs.h"
#include "pra_paging.h"

/**
 * @brief Page fault handler for FIFO algorithm.
 */
void pfh_fifo_isr(uint32_t error_code, page_fault_handler_result_t *result);
