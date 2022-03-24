#pragma once

#include <stddef.h>

typedef enum
{
    pra_fifo,
    pra_second_chance,
    pra_last_entry
} page_replacement_algorithm_e;

typedef void (*page_replacement_function_t)();

void register_page_replacement_algorithm(size_t index, page_replacement_function_t function);
page_replacement_function_t get_page_replacement_function(page_replacement_algorithm_e pra);
