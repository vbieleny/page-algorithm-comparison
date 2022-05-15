#include "pra_suite.h"
#include "pra_idt.h"
#include "pra_pfa.h"
#include "pra_pqueue.h"
#include "pra_malloc.h"
#include "pra_paging.h"
#include "pra_random.h"
#include "pra_io.h"
#include "pra_timer.h"

#ifndef DEFAULT_FORMAT
#define DEFAULT_FORMAT FORMAT_HUMAN_READABLE
#endif

static void test_reset_to_initial(page_replacement_algorithm_t algorithm, test_parameters_t parameters)
{
    page_replacement_algorithm_set_active(algorithm.function);
    page_replacement_algorithm_reset_time_taken();
    if (algorithm.init_function)
        algorithm.init_function();

    pfa_set_max_pages(parameters.pages_limit);
    pfa_free_all_pages();

    paging_reset_to_default();

    page_queue_clear();
    page_queue_set_capacity(parameters.pages_limit);

    user_memory_set_memory_size(parameters.allocation_spread);
    user_memory_free_all();

    pra_srand(parameters.seed);
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
            io_printf("V;%lu/%lu/%lu", parameters.pages_limit, parameters.allocation_spread, parameters.seed);
            for (size_t k = 0; k < configuration.algorithms_length; k++)
            {
                page_replacement_algorithm_t algorithm = get_page_replacement_algorithm(configuration.algorithms[k]);

                test_reset_to_initial(algorithm, parameters);

                io_stream_e previous_stream = io_get_stream();
                io_set_stream(IO_NONE);
                uint64_t start_timestamp = timestamp_scaled();
                execution.callback();
                uint64_t end_timestamp = timestamp_scaled();
                io_set_stream(previous_stream);

                if (algorithm.destroy_function)
                    algorithm.destroy_function();
                
                page_fault_statistics_t stats = paging_get_page_fault_statistics();
                io_printf(";%lu,%lu,%lu,%lu", stats.page_fault_without_victim, stats.page_fault_with_victim, stats.victim_accessed_count, stats.victim_dirty_count);

                uint64_t pra_time = page_replacement_algorithm_get_time_taken();
                uint64_t total_time = end_timestamp - start_timestamp;
                uint64_t overhead = (pra_time * 100ULL) / total_time;
                io_printf(",%lu", (uint32_t) overhead);
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

                test_reset_to_initial(algorithm, parameters);

                io_printf("%s | %s | %lu | %lu | %lu\n", execution.name, algorithm.name, parameters.pages_limit, parameters.allocation_spread, parameters.seed);
                
                uint64_t start_timestamp = timestamp_scaled();
                execution.callback();
                uint64_t end_timestamp = timestamp_scaled();

                if (algorithm.destroy_function)
                    algorithm.destroy_function();

                page_fault_statistics_t stats = paging_get_page_fault_statistics();
                io_printf("Page Faults (Without victim/With victim): %lu/%lu\n", stats.page_fault_without_victim, stats.page_fault_with_victim);
                io_printf("Accessed pages: %lu\n", stats.victim_accessed_count);
                io_printf("Dirty pages: %lu\n", stats.victim_dirty_count);

                uint64_t pra_time = page_replacement_algorithm_get_time_taken();
                uint64_t total_time = end_timestamp - start_timestamp;
                uint64_t overhead = (pra_time * 100ULL) / total_time;
                io_printf("Overhead: %lu%%\n\n", (uint32_t) overhead);
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
