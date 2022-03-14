#pragma once

#include <stdint.h>
#include <limits.h>

#define RAND_MAX ((INT_MAX) - 1)

int rand();
int rand_between(int min, int max);
void srand(unsigned int seed);
