/**
 * @file
 * @brief Implementation of Random page replacement algorithm.
 */

#pragma once

#include "pra_idt.h"
#include "pra_attrs.h"
#include "pra_paging.h"

/**
 * @brief Initialization function for a page fault handler.
 */
void pfh_random_init();

/**
 * @brief Destroy function for a page fault handler.
 */
void pfh_random_destroy();

/**
 * @brief Page fault handler for Random algorithm.
 */
void pfh_random_isr(uint32_t error_code, page_fault_handler_result_t *result);
