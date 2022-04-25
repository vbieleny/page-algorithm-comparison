#pragma once

#include <stddef.h>
#include <lib.h>

typedef void (*page_replacement_function_t)();

typedef enum
{
    pra_fifo,
    pra_second_chance,
    pra_last_entry
} page_replacement_algorithm_e;

typedef struct
{
    prac_name_t name;
    page_replacement_function_t function;
} page_replacement_algorithm_t;

void register_page_replacement_algorithm(size_t index, const char *name, page_replacement_function_t function);
page_replacement_algorithm_t get_page_replacement_algorithm(page_replacement_algorithm_e pra);
