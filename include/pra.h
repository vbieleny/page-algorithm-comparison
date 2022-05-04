#pragma once

#include <stddef.h>
#include <lib.h>
#include <idt.h>
#include <attrs.h>

typedef void (*page_replacement_function_t)(uint32_t);

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
void page_replacement_algorithm_set_active(page_replacement_function_t algorithm);
page_replacement_function_t page_replacement_algorithm_get_active();
uint64_t page_replacement_algorithm_get_time_taken();
void page_replacement_algorithm_reset_time_taken();

void pra_page_fault_handler(uint32_t error_code);
