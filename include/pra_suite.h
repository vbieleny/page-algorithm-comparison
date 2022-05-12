/**
 * @file
 * @brief This file provides functionality to run test suites and
 * generate output that can be evaluated.
 */

#pragma once

#include <stdint.h>
#include "pra_pfh.h"

typedef void (*test_execution_callback_t)();

typedef enum
{
    FORMAT_HUMAN_READABLE,
    FORMAT_PARSEABLE
} test_output_format_e;

typedef struct
{
    test_execution_callback_t callback;
    const char *name;
} test_execution_t;

typedef struct
{
    uint32_t pages_limit;
    uint32_t allocation_spread;
    uint32_t seed;
} test_parameters_t;

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
 */
void run_test_suite(test_configuration_t configuration);
