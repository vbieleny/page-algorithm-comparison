/**
 * @file
 * @brief This file provides functionality to run test suites and
 * generate output that can be evaluated.
 */

#pragma once

#include <stdint.h>
#include "pra_pfh.h"

/**
 * @brief Function for the test program implementation.
 */
typedef void (*test_execution_callback_t)();

/**
 * @brief Which test output format to use. This is set automatically and should not be changed.
 */
typedef enum
{
    FORMAT_HUMAN_READABLE,
    FORMAT_PARSEABLE
} test_output_format_e;

/**
 * @brief A structure defining the test program.
 * @param callback a test program function
 * @param name the name of the test program that will be shown in the results
 */
typedef struct
{
    test_execution_callback_t callback;
    const char *name;
} test_execution_t;

/**
 * @brief A structure defining the parameters for the test run.
 * @param pages_limit how many pages can be allocated in PFA before the software limit is reached
 * @param allocation_spread in how many pages can the user allocator allocate the memory in
 * @param seed a seed for the random number generator
 */
typedef struct
{
    uint32_t pages_limit;
    uint32_t allocation_spread;
    uint32_t seed;
} test_parameters_t;

/**
 * @brief A strucuture defining the test suite configuration.
 * @param tests the array of test programs to use in test suite
 * @param tests_length the length of the test programs array
 * @param algorithms the array of algorithm to use in test suite
 * @param algoritms_length the length of the algoritms array
 * @param parameters the array of parameters to use in test suite
 * @param parameters_length the length of the parameters array
 */
typedef struct
{
    test_execution_t *tests;
    size_t tests_length;
    page_replacement_algorithm_e *algorithms;
    size_t algorithms_length;
    test_parameters_t *parameters;
    size_t parameters_length;
} test_configuration_t;

/**
 * @brief Run the test suite specified as parameter.
 * 
 * Runs all combinations for specified test programs, parameters and
 * page replacement algorithms. The results will be written either to
 * terminal or serial output, depending on the default format.
 * 
 * @param configuration the test suite configuration to run
 */
void run_test_suite(test_configuration_t configuration);
