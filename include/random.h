#pragma once

#include <stdint.h>

uint32_t random_next();
uint32_t random_between(uint32_t min, uint32_t max);
void random_set_seed(uint32_t seed);
