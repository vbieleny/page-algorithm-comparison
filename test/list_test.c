#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "pra_unit.h"
#include "pra_list.h"
#include "pra_kmalloc.h"

typedef struct
{
    int test_number;
    char test_char;
} test_struct_t;

static uint8_t memory[1024 * 8];

static void init_memory()
{
    memset(memory, 0, sizeof(memory));
    kernel_memory_initialize(memory, ARRAY_LEN(memory));
}

static bool test_list_create()
{
    init_memory();

    list_t *list = list_create();
    PRA_ASSERT(list);
    PRA_ASSERT(list->size == 0);
    list_destroy(list);

    return true;
}

static bool test_list_create_initial_size()
{
    init_memory();

    list_t *list = list_create_with_initial_size(512);
    PRA_ASSERT(list);
    PRA_ASSERT(list_get_size(list) == 0);
    list_destroy(list);

    return true;
}

static bool test_list_append()
{
    init_memory();

    list_t *list = list_create();
    
    list_append(list, (void*) 42);
    list_append(list, (void*) 53);
    list_append(list, (void*) 1500);

    PRA_ASSERT(list_get_size(list) == 3);
    PRA_ASSERT((uintptr_t) list_get(list, 0) == 42);
    PRA_ASSERT((uintptr_t) list_get(list, 1) == 53);
    PRA_ASSERT((uintptr_t) list_get(list, 2) == 1500);

    list_destroy(list);

    return true;
}

static bool test_list_set()
{
    init_memory();

    list_t *list = list_create();
    
    list_append(list, (void*) 42);
    list_append(list, (void*) 53);
    list_append(list, (void*) 1500);

    list_set(list, 1, (void*) 11);
    list_set(list, 2, (void*) 22);

    PRA_ASSERT(list_get_size(list) == 3);
    PRA_ASSERT((uintptr_t) list_get(list, 0) == 42);
    PRA_ASSERT((uintptr_t) list_get(list, 1) == 11);
    PRA_ASSERT((uintptr_t) list_get(list, 2) == 22);

    list_destroy(list);

    return true;
}

static bool test_list_insert()
{
    init_memory();

    list_t *list = list_create();
    
    list_append(list, (void*) 42);
    list_append(list, (void*) 53);

    list_insert(list, 1, (void*) 100);

    PRA_ASSERT(list_get_size(list) == 3);
    PRA_ASSERT((uintptr_t) list_get(list, 0) == 42);
    PRA_ASSERT((uintptr_t) list_get(list, 1) == 100);
    PRA_ASSERT((uintptr_t) list_get(list, 2) == 53);

    list_destroy(list);

    return true;
}

static bool test_list_remove()
{
    init_memory();

    list_t *list = list_create();
    
    list_append(list, (void*) 42);
    list_append(list, (void*) 53);
    list_append(list, (void*) 1500);

    list_remove(list, 2);

    PRA_ASSERT(list_get_size(list) == 2);
    PRA_ASSERT((uintptr_t) list_get(list, 0) == 42);
    PRA_ASSERT((uintptr_t) list_get(list, 1) == 53);

    list_remove(list, 0);

    PRA_ASSERT(list_get_size(list) == 1);
    PRA_ASSERT((uintptr_t) list_get(list, 0) == 53);

    list_destroy(list);

    return true;
}

static bool test_list_index_of()
{
    init_memory();

    list_t *list = list_create();
    
    list_append(list, (void*) 17);
    list_append(list, (void*) 32);
    list_append(list, (void*) 7);

    PRA_ASSERT(list_index_of(list, (void*) 32) == 1);
    PRA_ASSERT(list_index_of(list, (void*) 7) == 2);
    PRA_ASSERT(list_index_of(list, (void*) 17) == 0);
    PRA_ASSERT(list_index_of(list, (void*) 42) == -1);

    list_destroy(list);

    return true;
}

static bool list_compare_struct(void *this, void *other)
{
    test_struct_t *this_struct = (test_struct_t*) this;
    test_struct_t *other_struct = (test_struct_t*) other;
    return this_struct->test_number == other_struct->test_number && this_struct->test_char == other_struct->test_char;
}

static bool test_list_index_of_with_comparison()
{
    init_memory();

    list_t *list = list_create();

    test_struct_t first_struct = { 42, 'X' };
    test_struct_t second_struct = { 1, 'A' };
    test_struct_t third_struct = { 333, 'Q' };
    test_struct_t fourth_struct = { 123, 'A' };
    
    list_append(list, &first_struct);
    list_append(list, &second_struct);
    list_append(list, &third_struct);

    test_struct_t struct_to_find = { 1, 'A' };
    PRA_ASSERT(&struct_to_find != &second_struct);
    PRA_ASSERT(list_index_of_by_comparison(list, &struct_to_find, list_compare_struct) == 1);
    PRA_ASSERT(list_index_of_by_comparison(list, &third_struct, list_compare_struct) == 2);
    PRA_ASSERT(list_index_of_by_comparison(list, &fourth_struct, list_compare_struct) == -1);

    list_destroy(list);

    return true;
}

static bool test_list_combined()
{
    init_memory();

    list_t *list = list_create();
    
    list_append(list, (void*) 1);
    list_append(list, (void*) 2);
    list_append(list, (void*) 3);

    list_remove(list, 0);

    PRA_ASSERT(list_get_size(list) == 2);
    PRA_ASSERT((uintptr_t) list_get(list, 0) == 2);
    PRA_ASSERT((uintptr_t) list_get(list, 1) == 3);

    list_set(list, 1, (void*) 100);

    PRA_ASSERT((uintptr_t) list_get(list, 1) == 100);

    list_insert(list, 0, (void*) 42);
    list_insert(list, 2, (void*) 200);

    PRA_ASSERT(list_get_size(list) == 4);
    PRA_ASSERT((uintptr_t) list_get(list, 0) == 42);
    PRA_ASSERT((uintptr_t) list_get(list, 1) == 2);
    PRA_ASSERT((uintptr_t) list_get(list, 2) == 200);
    PRA_ASSERT((uintptr_t) list_get(list, 3) == 100);

    PRA_ASSERT(list_index_of(list, (void*) 200) == 2);
    PRA_ASSERT(list_index_of(list, (void*) 42) == 0);
    PRA_ASSERT(list_index_of(list, (void*) 2000) == -1);

    list_destroy(list);

    return true;
}

static bool all_tests()
{
    PRA_RUN_TEST(test_list_create);
    PRA_RUN_TEST(test_list_create_initial_size);
    PRA_RUN_TEST(test_list_append);
    PRA_RUN_TEST(test_list_set);
    PRA_RUN_TEST(test_list_insert);
    PRA_RUN_TEST(test_list_remove);
    PRA_RUN_TEST(test_list_index_of);
    PRA_RUN_TEST(test_list_index_of_with_comparison);
    PRA_RUN_TEST(test_list_combined);
    return true;
}

int main(int argc, char *argv[])
{
    all_tests();
    return 0;
}
