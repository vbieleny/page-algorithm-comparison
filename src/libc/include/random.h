#pragma once

#include <stdint.h>
#include <limits.h>

#define RAND_MAX ((INT_MAX) - 1)

int rand();
void srand(unsigned int seed);
