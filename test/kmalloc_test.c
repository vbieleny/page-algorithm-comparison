#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "pra_unit.h"
#include "kmalloc.h"
#include "lib.h"

#define ASSERT_MEM_FREE do { PRA_ASSERT(kernel_memory_get_free_mem() == ARRAY_LEN(memory)); PRA_ASSERT(kernel_memory_get_free_blocks() == 1); } while (0)

static uint8_t memory[1024];

static void init_memory()
{
    memset(memory, 0, sizeof(memory));
    kernel_memory_initialize(memory, ARRAY_LEN(memory));
}

static bool test_kmalloc_allocate_int()
{
    init_memory();
    ASSERT_MEM_FREE;

    int *value = kernel_memory_allocate(sizeof(int), 1);
    PRA_ASSERT(value);

    PRA_ASSERT(kernel_memory_get_free_blocks() == 1);
    return true;
}

static bool test_kmalloc_allocate_free_int()
{
    init_memory();
    ASSERT_MEM_FREE;

    int *value = kernel_memory_allocate(sizeof(int), 1);
    PRA_ASSERT(value);
    kernel_memory_free(value);

    ASSERT_MEM_FREE;
    return true;
}

static bool test_kmalloc_allocate_free_multiple()
{
    init_memory();
    ASSERT_MEM_FREE;

    int *value1 = kernel_memory_allocate(sizeof(int), 1);
    PRA_ASSERT(value1);
    int *value2 = kernel_memory_allocate(sizeof(int) * 7, 1);
    PRA_ASSERT(value2);
    int *value3 = kernel_memory_allocate(sizeof(int) * 4, 1);
    PRA_ASSERT(value3);
    kernel_memory_free(value3);
    kernel_memory_free(value1);
    kernel_memory_free(value2);

    ASSERT_MEM_FREE;
    return true;
}

static bool test_kmalloc_allocate_free_multiple_mixed()
{
    init_memory();
    ASSERT_MEM_FREE;

    int *value1 = kernel_memory_allocate(sizeof(int) * 15, 1);
    PRA_ASSERT(value1);
    int *value2 = kernel_memory_allocate(sizeof(int) * 5, 1);
    PRA_ASSERT(value2);
    kernel_memory_free(value2);
    int *value3 = kernel_memory_allocate(sizeof(int) * 10, 1);
    PRA_ASSERT(value3);
    kernel_memory_free(value3);
    kernel_memory_free(value1);

    ASSERT_MEM_FREE;
    return true;
}

static bool test_kmalloc_allocate_int_align()
{
    init_memory();
    ASSERT_MEM_FREE;

    int *value = kernel_memory_allocate(sizeof(int), 2);
    PRA_ASSERT(value);

    PRA_ASSERT(kernel_memory_get_free_blocks() == 1);
    return true;
}

static bool test_kmalloc_allocate_free_int_align()
{
    init_memory();
    ASSERT_MEM_FREE;

    int *value = kernel_memory_allocate(sizeof(int), 4);
    PRA_ASSERT(value);
    kernel_memory_free(value);

    ASSERT_MEM_FREE;
    return true;
}

static bool test_kmalloc_allocate_free_multiple_align()
{
    init_memory();
    ASSERT_MEM_FREE;

    int *value1 = kernel_memory_allocate(sizeof(int), 32);
    PRA_ASSERT(value1);
    int *value2 = kernel_memory_allocate(sizeof(int) * 7, 8);
    PRA_ASSERT(value2);
    int *value3 = kernel_memory_allocate(sizeof(int) * 4, 16);
    PRA_ASSERT(value3);
    kernel_memory_free(value3);
    kernel_memory_free(value1);
    kernel_memory_free(value2);

    ASSERT_MEM_FREE;
    return true;
}

static bool test_kmalloc_allocate_free_multiple_mixed_align()
{
    init_memory();
    ASSERT_MEM_FREE;

    int *value1 = kernel_memory_allocate(sizeof(int) * 15, 64);
    PRA_ASSERT(value1);
    int *value2 = kernel_memory_allocate(sizeof(int) * 5, 32);
    PRA_ASSERT(value2);
    kernel_memory_free(value2);
    int *value3 = kernel_memory_allocate(sizeof(int) * 10, 1);
    PRA_ASSERT(value3);
    kernel_memory_free(value3);
    kernel_memory_free(value1);

    ASSERT_MEM_FREE;
    return true;
}

static bool test_kmalloc_allocate_free_whole_memory()
{
    init_memory();
    ASSERT_MEM_FREE;

    void *value1 = kernel_memory_allocate(ARRAY_LEN(memory) - kernel_memory_get_block_size(), 1);
    PRA_ASSERT(value1);
    uint8_t *value2 = kernel_memory_allocate(sizeof(uint8_t), 1);
    PRA_ASSERT(!value2);
    kernel_memory_free(value1);
    value2 = kernel_memory_allocate(sizeof(uint8_t), 1);
    PRA_ASSERT(value2);
    kernel_memory_free(value2);

    ASSERT_MEM_FREE;
    return true;
}

static bool test_kmalloc_allocate_free_whole_memory_split()
{
    init_memory();
    ASSERT_MEM_FREE;

    void *value1 = kernel_memory_allocate(ARRAY_LEN(memory) / 2 - kernel_memory_get_block_size(), 1);
    PRA_ASSERT(value1);
    void *value2 = kernel_memory_allocate(ARRAY_LEN(memory) / 2 - kernel_memory_get_block_size(), 1);
    PRA_ASSERT(value2);
    uint8_t *value3 = kernel_memory_allocate(sizeof(uint8_t), 1);
    PRA_ASSERT(!value3);
    kernel_memory_free(value1);
    value3 = kernel_memory_allocate(sizeof(uint8_t), 1);
    PRA_ASSERT(value3);
    kernel_memory_free(value2);
    kernel_memory_free(value3);

    ASSERT_MEM_FREE;
    return true;
}

static bool all_tests()
{
    PRA_RUN_TEST(test_kmalloc_allocate_int);
    PRA_RUN_TEST(test_kmalloc_allocate_free_int);
    PRA_RUN_TEST(test_kmalloc_allocate_free_multiple);
    PRA_RUN_TEST(test_kmalloc_allocate_free_multiple_mixed);
    PRA_RUN_TEST(test_kmalloc_allocate_int_align);
    PRA_RUN_TEST(test_kmalloc_allocate_free_int_align);
    PRA_RUN_TEST(test_kmalloc_allocate_free_multiple_align);
    PRA_RUN_TEST(test_kmalloc_allocate_free_multiple_mixed_align);
    PRA_RUN_TEST(test_kmalloc_allocate_free_whole_memory);
    PRA_RUN_TEST(test_kmalloc_allocate_free_whole_memory_split);
    return true;
}

int main(int argc, char *argv[])
{
    all_tests();
    return 0;
}
