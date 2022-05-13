/**
 * @file
 * @brief Standard library functions implementation.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Returns size of statically allocated array (in elements).
 */
#define ARRAY_LEN(array) (sizeof((array)) / sizeof((array)[0]))

typedef char prac_name_t[128];

/**
 * @brief Fill memory of certain length with specified value.
 * @param memory starting address of memory
 * @param value value that will be filled
 * @param length length of the memory we want to fill
 */
void pra_memset(void *memory, uint8_t value, size_t length);

/**
 * @brief Copies certain amount of memory from one address to another.
 * @param destination destination memory address
 * @param source source memory address
 * @param length the length of source memory that will be copied
 * @return detination memory address
 */
void* pra_memcpy(void *destination, const void *source, size_t length);

/**
 * @brief Checks whether a character is a digit.
 * @param digit parameter to check
 * @return 1 if the character is a digit, otherwise 0
 */
int pra_isdigit(int digit);
