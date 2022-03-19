#pragma once

#include <types.h>

u32 run_test(const char *test_name, const char *isr_name, void *pra_isr, void (*test_function)(), u32 pages_limit, u32 allocation_spread);
