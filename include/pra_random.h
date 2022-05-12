#pragma once

#include <stdint.h>
#include <stddef.h>

uint32_t pra_rand();
uint32_t pra_rand_between(uint32_t min, uint32_t max);
void pra_srand(uint32_t seed);
void pra_fill_random(int *array, size_t size, uint32_t min, uint32_t max);
