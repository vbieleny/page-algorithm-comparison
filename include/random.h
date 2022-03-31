#pragma once

#include <stdint.h>
#include <stddef.h>

uint32_t rand();
uint32_t rand_between(uint32_t min, uint32_t max);
void srand(uint32_t seed);
void prac_fill_random(int *array, size_t size, uint32_t min, uint32_t max);
