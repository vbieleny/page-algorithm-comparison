#include <test_runner.h>
#include <pra.h>
#include <pfh_fifo.h>
#include <pfh_second.h>
#include <io.h>
#include <malloc.h>
#include <random.h>

typedef struct linked_list
{
    struct linked_list *next;
    int value;
} linked_list_t;

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void test_sort()
{
    int numbers[16];
    int numbers_count = sizeof(numbers) / sizeof(numbers[0]);

    prac_fill_random(numbers, numbers_count, 0, 100);

    linked_list_t *root = (linked_list_t*) user_memory_random_allocate(sizeof(linked_list_t));
    linked_list_t *next = root;
    for (int i = 0; i < numbers_count; i++)
    {
        next->value = numbers[i];
        if (i == numbers_count - 1)
            break;
        next->next = (linked_list_t*) user_memory_random_allocate(sizeof(linked_list_t));
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

    next = root;
    for (int i = 0; i < numbers_count; i++, next = next->next)
    {
        const char *format = i == numbers_count - 1 ? "%d" : "%d ";
        io_printf(format, next->value);
    }
    io_printf("\n");
}

void setup()
{
    test_execution_t executions[2];
    executions[0].name = "Linked List Sort";
    executions[0].callback = test_sort;
    executions[1].name = "Another Sort";
    executions[1].callback = test_sort;

    page_replacement_algorithm_e algorithms[2];
    algorithms[0] = pra_fifo;
    algorithms[1] = pra_second_chance;

    test_parameters_t parameters[2];
    parameters[0].pages_limit = 6;
    parameters[0].allocation_spread = 8;
    parameters[0].seed = 1;
    parameters[1].pages_limit = 7;
    parameters[1].allocation_spread = 8;
    parameters[1].seed = 1;

    test_configuration_t configuration =
    {
        .tests = executions,
        .tests_length = 2,
        .algorithms = algorithms,
        .algorithms_length = 2,
        .parameters = parameters,
        .parameters_length = 2
    };

    run_test_suite(configuration);
}