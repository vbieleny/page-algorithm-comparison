#pragma once

#include <stdint.h>

extern const unsigned long int RAND_MAX;

int rand();
void srand(unsigned int seed);
