#pragma once

#include <stdint.h>

uint32_t run_test(const char *test_name, const char *isr_name, void *pra_isr, void (*test_function)(), uint32_t pages_limit, uint32_t allocation_spread);
