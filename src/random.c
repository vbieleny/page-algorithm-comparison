#include <random.h>
#include <limits.h>

static const uint32_t RAND_MAX = (INT_MAX) - 1;

static uint32_t next = 1;

uint32_t rand()
{
    // A very simple LCG (linear congruential generator)
    next = next * 1103515245 + 12345;
    return (uint32_t) next % (RAND_MAX + 1);
}

uint32_t rand_between(uint32_t min, uint32_t max)
{
    return (rand() % (max + 1 - min)) + min;
}

void srand(uint32_t seed)
{
    next = seed;
}

void prac_fill_random(int *array, size_t size, uint32_t min, uint32_t max)
{
    for (size_t i = 0; i < size; i++)
        array[i] = rand_between(min, max);
}
