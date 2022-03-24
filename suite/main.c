#include <test_runner.h>
#include <pfh_fifo.h>
#include <pfh_second.h>
#include <io.h>
#include <malloc.h>

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
    int numbers[16] = { 35, 2, 19, 17, 5, 7, 1, 53, 11, 9, 31, 16, 6, 27, 24, 99 };
    int numbers_count = sizeof(numbers) / sizeof(numbers[0]);
    
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
    run_test("Linked List Sort", "FIFO", &pfh_fifo_isr, &test_sort, 6, 8);
    run_test("Linked List Sort", "Second Chance", &pfh_second_isr, &test_sort, 6, 8);
}
