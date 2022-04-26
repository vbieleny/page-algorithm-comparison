#include <test_runner.h>
#include <idt.h>
#include <pfa.h>
#include <pqueue.h>
#include <malloc.h>
#include <paging.h>
#include <random.h>
#include <io.h>

#ifndef DEFAULT_FORMAT
#define DEFAULT_FORMAT FORMAT_HUMAN_READABLE
#endif

static void test_reset_to_initial(page_replacement_function_t algorithm, test_parameters_t parameters)
{
    idt_set_descriptor(14, algorithm, 0x8e);

    pfa_set_max_pages(parameters.pages_limit);
    pfa_free_all_pages();

    paging_reset_to_default();

    page_queue_clear();
    page_queue_set_capacity(parameters.pages_limit);

    user_memory_set_memory_size(parameters.allocation_spread);
    user_memory_free_all();

    srand(parameters.seed);
}

static void run_test_suite_parseable(test_configuration_t configuration)
{
    io_printf("I");
    for (size_t k = 0; k < configuration.algorithms_length; k++)
    {
        page_replacement_algorithm_t algorithm = get_page_replacement_algorithm(configuration.algorithms[k]);
        io_printf(";%s", algorithm.name);
    }
    io_printf("\n");
    for (size_t i = 0; i < configuration.tests_length; i++)
    {
        test_execution_t execution = configuration.tests[i];
        io_printf("T;%s\n", execution.name);
        for (size_t j = 0; j < configuration.parameters_length; j++)
        {
            test_parameters_t parameters = configuration.parameters[j];
            io_printf("V;%d/%d/%d", parameters.pages_limit, parameters.allocation_spread, parameters.seed);
            for (size_t k = 0; k < configuration.algorithms_length; k++)
            {
                page_replacement_algorithm_t algorithm = get_page_replacement_algorithm(configuration.algorithms[k]);

                test_reset_to_initial(algorithm.function, parameters);

                io_stream_e previous_stream = io_get_stream();
                io_set_stream(IO_NONE);
                execution.callback();
                io_set_stream(previous_stream);
                
                uint32_t page_faults = paging_get_page_fault_count();
                io_printf(";%d", page_faults);
            }
            io_printf("\n");
        }
    }
    io_printf("E\n");
}

static void run_test_suite_human_readable(test_configuration_t configuration)
{
    for (size_t i = 0; i < configuration.tests_length; i++)
    {
        test_execution_t execution = configuration.tests[i];
        for (size_t j = 0; j < configuration.parameters_length; j++)
        {
            test_parameters_t parameters = configuration.parameters[j];
            for (size_t k = 0; k < configuration.algorithms_length; k++)
            {
                page_replacement_algorithm_t algorithm = get_page_replacement_algorithm(configuration.algorithms[k]);

                test_reset_to_initial(algorithm.function, parameters);

                io_printf("%s | %s | %d | %d | %d\n", execution.name, algorithm.name, parameters.pages_limit, parameters.allocation_spread, parameters.seed);
                execution.callback();
                uint32_t page_faults = paging_get_page_fault_count();
                io_printf("Page Faults: %d\n\n", page_faults);
            }
        }
    }
}

void run_test_suite(test_configuration_t configuration)
{
    if (DEFAULT_FORMAT == FORMAT_PARSEABLE)
        run_test_suite_parseable(configuration);
    else
        run_test_suite_human_readable(configuration);
}
