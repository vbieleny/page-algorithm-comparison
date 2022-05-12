#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "pra_unit.h"
#include "pra_kmalloc.h"
#include "pra_malloc.h"
#include "pra_pfa.h"
#include "pra_lib.h"

static uint8_t kernel_memory[1024 * 32];
static uint8_t user_memory[1024 * 32];

static void init_memory()
{
    memset(kernel_memory, 0, sizeof(kernel_memory));
    kernel_memory_initialize(kernel_memory, ARRAY_LEN(kernel_memory));
    pfa_initialize(user_memory);
    user_memory_initialize();
    user_memory_free_all();
}

static bool test_malloc_allocate_basic()
{
    init_memory();

    PRA_ASSERT(user_memory_random_allocate(256));

    return true;
}

static bool test_malloc_allocate_set_memory_size()
{
    init_memory();

    PRA_ASSERT(user_memory_random_allocate(128));
    user_memory_set_memory_size(4);
    PRA_ASSERT(user_memory_random_allocate(8192));

    return true;
}

static bool test_malloc_allocate_multiple_small_chunks()
{
    init_memory();

    user_memory_set_memory_size(1);

    for (size_t i = 0; i < 16; i++)
        PRA_ASSERT(user_memory_random_allocate(16));

    return true;
}

static bool all_tests()
{
    PRA_RUN_TEST(test_malloc_allocate_basic);
    PRA_RUN_TEST(test_malloc_allocate_set_memory_size);
    PRA_RUN_TEST(test_malloc_allocate_multiple_small_chunks);
    return true;
}

int main(int argc, char *argv[])
{
    all_tests();
    return 0;
}
