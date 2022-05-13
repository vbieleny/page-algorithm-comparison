/**
 * @file
 * @brief Unit testing functionality for writing tests for the framework.
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>

#define PRA_STRINGIZE_DETAIL(x) #x
#define PRA_STRINGIZE(x) PRA_STRINGIZE_DETAIL(x)

/**
 * @brief Asserts the condition. If the assertion fails, an error message will be printed to the standard output.
 * @param test the condition to assert
 */
#define PRA_ASSERT(test) do { if (!(test)) { printf("assert: %s:%d: %s: Assertion '%s' failed.\n", __FILE__, __LINE__, __func__, #test); return false; } } while (0)

/**
 * @brief Asserts the condition. If the assertion fails, a custom error message will be printed to the standard output.
 * @param message the custom message to print if the assertion fails
 * @param test the condition to assert
 */
#define PRA_ASSERT_MESSAGE(message, test) do { if (!(test)) { printf("%s\n", (message)); return false; } } while (0)

/**
 * @brief Runs the specified test and prints the result of the test to the standard output.
 * @param test the function containing the test
 */
#define PRA_RUN_TEST(test) do { bool success = test(); printf("%s:%s\n", #test, success ? "PASS" : "FAIL"); if (!success) return false; } while (0)
