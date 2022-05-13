/**
 * @file
 * @brief Implementation of Second Chance page replacement algorithm.
 */

#pragma once

#include "pra_idt.h"
#include "pra_attrs.h"
#include "pra_paging.h"

/**
 * @brief Page fault handler for Second Chance algorithm.
 */
void pfh_second_isr(uint32_t error_code, page_fault_handler_result_t *result);
