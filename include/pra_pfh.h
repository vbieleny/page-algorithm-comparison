/**
 * @file
 * @brief Provides page fault handling functionality, registering new PFH and selecting active ones.
 */

#pragma once

#include <stddef.h>
#include "pra_lib.h"
#include "pra_idt.h"
#include "pra_attrs.h"
#include "pra_paging.h"

/**
 * @brief Initialization function for a page fault handler.
 */
typedef void (*page_replacement_init_function_t)();

/**
 * @brief Destroy function for a page fault handler.
 */
typedef void (*page_replacement_destroy_function_t)();

/**
 * @brief Page fault handler for custom page replacement algorithm.
 */
typedef void (*page_replacement_function_t)(uint32_t, page_fault_handler_result_t*);

/**
 * @brief Enum containing a list of all built-in page replacement algorithms.
 */
typedef enum
{
    pra_fifo,
    pra_second_chance,
    pra_random,
    pra_last_entry
} page_replacement_algorithm_e;

/**
 * @brief Structure defining a page replacement algorithm.
 */
typedef struct
{
    prac_name_t name;
    page_replacement_init_function_t init_function;
    page_replacement_destroy_function_t destroy_function;
    page_replacement_function_t function;
} page_replacement_algorithm_t;

/**
 * @brief Registers a new custom page replacement algorithm.
 * @param index where to place new page replacement algorithm, must be bigger than pra_last_entry
 * @param name the name of the page replacement algorithm that will be shown in results
 * @param init_function the init function for the PFH (page fault handler)
 * @param destroy_function the destroy function for the PFH (page fault handler)
 * @param function the page fault handler function containing the page replacement algorithm
 */
void register_page_replacement_algorithm(size_t index, const char *name, page_replacement_init_function_t init_function, page_replacement_destroy_function_t destroy_function, page_replacement_function_t function);

/**
 * @brief Gets the page replacement algorithm definition.
 * @param pra which page replacement function to return
 * @return structure defining the page replacement algorithm
 */
page_replacement_algorithm_t get_page_replacement_algorithm(page_replacement_algorithm_e pra);

/**
 * @brief Sets specified page replacement algorithm as active. This means that it will be used when a page fault occurrs.
 * @param algorithm which page replacement algorithm to set as active
 */
void page_replacement_algorithm_set_active(page_replacement_function_t algorithm);

/**
 * @brief Returns the currently active page replacement algorithm.
 * @return structure containing definition of active page replacement algorithm
 */
page_replacement_function_t page_replacement_algorithm_get_active();

/**
 * @brief Returns the total time taken by page fault handler from the last reset. Used for measuring algorithm overhead.
 * @return 64-bit value containing how many instructions happened since last reset
 */
uint64_t page_replacement_algorithm_get_time_taken();

/**
 * @brief Resets the total time taken by page fault handler to 0. This is called automatically for each testing program.
 */
void page_replacement_algorithm_reset_time_taken();

/**
 * @brief General page fault handler that will get called from when a page fault occurs.
 * It will call active page fault handler and process all result data. Should not be called manually.
 * @param error_code error code of the occurred page fault
 */
void pra_page_fault_handler(uint32_t error_code);
