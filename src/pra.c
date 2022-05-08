#include <pra.h>
#include <kmalloc.h>
#include <string_utils.h>
#include <timer.h>

static const size_t DEFAULT_PRA_FUNCTIONS_SIZE = 16;

static page_replacement_function_t pra_active_algorithm;
static uint64_t pra_time_taken;

static page_replacement_algorithm_t *pra_functions;
static size_t pra_capacity;
static size_t pra_size;

void register_page_replacement_algorithm(size_t index, const char *name, page_replacement_init_function_t init_function, page_replacement_destroy_function_t destroy_function, page_replacement_function_t function)
{
    if (!pra_functions)
    {
        pra_capacity = DEFAULT_PRA_FUNCTIONS_SIZE;
        pra_functions = kernel_memory_allocate(pra_capacity * sizeof(page_replacement_algorithm_t), 1);
    }
    if (pra_size >= pra_capacity)
    {
        pra_capacity *= 2;
        pra_functions = kernel_memory_reallocate(pra_functions, pra_capacity * sizeof(page_replacement_algorithm_t), 1);
    }
    pra_size++;
    
    page_replacement_algorithm_t algorithm;
    pra_memset(&algorithm, 0, sizeof(page_replacement_algorithm_t));

    strcpy(algorithm.name, name);
    algorithm.init_function = init_function;
    algorithm.destroy_function = destroy_function;
    algorithm.function = function;

    pra_functions[index] = algorithm;
}

page_replacement_algorithm_t get_page_replacement_algorithm(page_replacement_algorithm_e pra)
{
    return pra_functions[pra];
}

void page_replacement_algorithm_set_active(page_replacement_function_t algorithm)
{
    pra_active_algorithm = algorithm;
}

page_replacement_function_t page_replacement_algorithm_get_active()
{
    return pra_active_algorithm;
}

uint64_t page_replacement_algorithm_get_time_taken()
{
    return pra_time_taken;
}

void page_replacement_algorithm_reset_time_taken()
{
    pra_time_taken = 0;
}

void pra_page_fault_handler(uint32_t error_code)
{
    page_fault_handler_result_t result;
    pra_memset(&result, 0, sizeof(result));
    uint64_t start_timestamp = timestamp_scaled();
    pra_active_algorithm(error_code, &result);
    uint64_t end_timestamp = timestamp_scaled();
    pra_time_taken += end_timestamp - start_timestamp;
    paging_add_result_to_statistics(&result);
}
