#include <terminal.h>
#include <lib.h>
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

void test_sort()
{
    int numbers[16] = { 35, 2, 19, 17, 5, 7, 1, 53, 11, 9, 31, 16, 6, 27, 24, 99 };
    int numbers_count = sizeof(numbers) / sizeof(numbers[0]);
    
    linked_list_t *root = (linked_list_t*) memory_random_allocate(sizeof(linked_list_t));
    linked_list_t *next = root;
    for (int i = 0; i < numbers_count; i++)
    {
        next->value = numbers[i];
        if (i == numbers_count - 1)
            break;
        next->next = (linked_list_t*) memory_random_allocate(sizeof(linked_list_t));
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
        terminal_printf("%d ", next->value);
}
