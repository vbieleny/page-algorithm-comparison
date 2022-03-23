#include <random.h>
#include <limits.h>

static const uint32_t RAND_MAX = (INT_MAX) - 1;

static uint32_t next = 1;

uint32_t random_next()
{
    // A very simple LCG (linear congruential generator)
    next = next * 1103515245 + 12345;
    return (uint32_t) next % (RAND_MAX + 1);
}

uint32_t random_between(uint32_t min, uint32_t max)
{
    return (random_next() % (max + 1 - min)) + min;
}

void random_set_seed(uint32_t seed)
{
    next = seed;
}
