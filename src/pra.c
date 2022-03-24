#include <pra.h>
#include <kmalloc.h>

static const size_t DEFAULT_PRA_FUNCTIONS_SIZE = 16;

static page_replacement_function_t *pra_functions;
static size_t pra_capacity;
static size_t pra_size;

void register_page_replacement_algorithm(size_t index, page_replacement_function_t function)
{
    if (!pra_functions)
    {
        pra_capacity = DEFAULT_PRA_FUNCTIONS_SIZE;
        pra_functions = kernel_memory_allocate(pra_capacity * sizeof(page_replacement_function_t), 1);
    }
    if (pra_size >= pra_capacity)
    {
        pra_capacity *= 2;
        pra_functions = kernel_memory_reallocate(pra_functions, pra_capacity * sizeof(page_replacement_function_t), 1);
    }
    pra_size++;
    pra_functions[index] = function;
}

page_replacement_function_t get_page_replacement_function(page_replacement_algorithm_e pra)
{
    return pra_functions[pra];
}
