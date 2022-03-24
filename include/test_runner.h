#pragma once

#include <stdint.h>
#include <pra.h>

uint32_t run_test(const char *test_name, const char *isr_name, page_replacement_algorithm_e pra, void (*test_function)(), uint32_t pages_limit, uint32_t allocation_spread);
