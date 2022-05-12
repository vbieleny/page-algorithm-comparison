#include "pra_io.h"
#include "pra_lib.h"
#include "pra_malloc.h"
#include "pra_pfh_fifo.h"
#include "pra_pfh_second.h"
#include "pra_pfh.h"
#include "pra_random.h"
#include "pra_suite.h"

typedef struct linked_list
{
    struct linked_list *next;
    int value;
} linked_list_t;

static const int NUMBERS_LENGTH = 128;

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void test_selection_sort()
{
    int numbers[NUMBERS_LENGTH];
    int numbers_count = ARRAY_LEN(numbers);

    pra_fill_random(numbers, numbers_count, 0, 100);

    linked_list_t *root = (linked_list_t *) user_memory_random_allocate(sizeof(linked_list_t));
    linked_list_t *next = root;

    for (int i = 0; i < numbers_count; i++)
    {
        int value = numbers[i];
        next->value = value;
        if (i == numbers_count - 1)
            break;
        next->next = (linked_list_t *) user_memory_random_allocate(sizeof(linked_list_t));
        next = next->next;
    }

    next = root;
    for (int i = 0; i < numbers_count - 1; i++, next = next->next)
    {
        linked_list_t *min_item = next;
        linked_list_t *nested = next->next;
        for (int j = i + 1; j < numbers_count; j++, nested = nested->next)
        {
            if (nested->value < min_item->value)
                min_item = nested;
        }
        swap(&(min_item->value), &(next->value));
    }
}

static void test_bubble_sort()
{
    int numbers[NUMBERS_LENGTH];
    int numbers_count = ARRAY_LEN(numbers);

    pra_fill_random(numbers, numbers_count, 0, 100);

    linked_list_t *root = (linked_list_t *) user_memory_random_allocate(sizeof(linked_list_t));
    linked_list_t *next = root;
    for (int i = 0; i < numbers_count; i++)
    {
        next->value = numbers[i];
        if (i == numbers_count - 1)
            break;
        next->next = (linked_list_t *) user_memory_random_allocate(sizeof(linked_list_t));
        next = next->next;
    }

    for (int i = 0; i < numbers_count; i++)
    {
        next = root;
        for (int j = 0; j < numbers_count - 1; j++, next = next->next)
        {
            if (next->value > next->next->value)
                swap(&(next->value), &(next->next->value));
        }
    }
}

void setup()
{
    test_execution_t executions[2];
    executions[0].name = "Bubble Sort";
    executions[0].callback = test_bubble_sort;
    executions[1].name = "Selection Sort";
    executions[1].callback = test_selection_sort;

    page_replacement_algorithm_e algorithms[3];
    algorithms[0] = pra_fifo;
    algorithms[1] = pra_second_chance;
    algorithms[2] = pra_random;

    test_parameters_t parameters[3];
    parameters[0].pages_limit = 10;
    parameters[0].allocation_spread = 15;
    parameters[0].seed = 1;
    parameters[1].pages_limit = 20;
    parameters[1].allocation_spread = 25;
    parameters[1].seed = 1;
    parameters[2].pages_limit = 30;
    parameters[2].allocation_spread = 35;
    parameters[2].seed = 1;

    test_configuration_t configuration =
    {
        .tests = executions,
        .tests_length = ARRAY_LEN(executions),
        .algorithms = algorithms,
        .algorithms_length = ARRAY_LEN(algorithms),
        .parameters = parameters,
        .parameters_length = ARRAY_LEN(parameters)
    };

    run_test_suite(configuration);
}
