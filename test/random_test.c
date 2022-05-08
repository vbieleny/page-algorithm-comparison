#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "pra_unit.h"
#include "random.h"
#include "lib.h"

static bool test_random_basic()
{
    for (size_t i = 0; i < 10; i++)
        pra_rand();

    return true;
}

static bool test_random_between()
{
    {
        uint32_t min = 7500;
        uint32_t max = 125000;
        for (size_t i = 0; i < 10; i++)
        {
            uint32_t number = pra_rand_between(min, max);
            PRA_ASSERT(number >= min && number <= max);
        }
    }

    {
        uint32_t min = 0;
        uint32_t max = 1;
        for (size_t i = 0; i < 10; i++)
        {
            uint32_t number = pra_rand_between(min, max);
            PRA_ASSERT(number >= min && number <= max);
        }
    }

    {
        uint32_t min = 100;
        uint32_t max = 100;
        for (size_t i = 0; i < 10; i++)
        {
            uint32_t number = pra_rand_between(min, max);
            PRA_ASSERT(number >= min && number <= max);
        }
    }

    return true;
}

static bool test_random_fill()
{
    int numbers[16];
    memset(numbers, 0, sizeof(numbers));

    uint32_t min = 100;
    uint32_t max = 200;
    pra_fill_random(numbers, ARRAY_LEN(numbers), min, max);

    for (size_t i = 0; i < ARRAY_LEN(numbers); i++)
        PRA_ASSERT(numbers[i] >= min && numbers[i] <= max);

    return true;
}

static bool all_tests()
{
    PRA_RUN_TEST(test_random_basic);
    PRA_RUN_TEST(test_random_between);
    PRA_RUN_TEST(test_random_fill);
    return true;
}

int main(int argc, char *argv[])
{
    pra_srand(42);
    all_tests();
    return 0;
}
