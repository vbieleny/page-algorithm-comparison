#include <random.h>

static unsigned long int next = 1;

int rand()
{
    // A very simple LCG (linear congruential generator)
    next = next * 1103515245 + 12345;
    return (unsigned int) next % (RAND_MAX + 1);
}

void srand(unsigned int seed)
{
    next = seed;
}
