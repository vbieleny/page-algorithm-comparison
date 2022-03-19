#include <random.h>
#include <limits.h>

static const u32 RAND_MAX = (INT_MAX) - 1;

static u32 next = 1;

u32 random_next()
{
    // A very simple LCG (linear congruential generator)
    next = next * 1103515245 + 12345;
    return (u32) next % (RAND_MAX + 1);
}

u32 random_between(u32 min, u32 max)
{
    return (random_next() % (max + 1 - min)) + min;
}

void random_set_seed(u32 seed)
{
    next = seed;
}
