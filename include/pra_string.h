/**
 * @file
 * @brief Standard library functions for string operations.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Returns the length of the string without the null terminating character.
 * @param str the string we want to find the length of
 * @return length of the string
 */
size_t pra_strlen(const char *str);

/**
 * @brief Copies the string from from source to destination.
 * @param destination where to copy the string
 * @param source the string to copy
 * @return the destination string
 */
char* pra_strcpy(char *destination, const char *source);

/**
 * @brief Concatenates the source and destination string and stores it it the destination string.
 * @param destination the destination string where the concatenation result will be stored
 * @param source the source string
 * @return the destination string
 */
char* pra_strcat(char *destination, const char *source);

/**
 * @brief Converts a number to a string.
 * @param value the number to convert
 * @param result where to store the converted number
 * @param base which base to use, for decimal numbers use 10
 * @return the result string
 */
char* pra_itoa(int32_t value, char *result, int32_t base);

/**
 * @brief Advanced conversion from a number to a string. Used in printf implementation.
 * @param i the number to convert
 * @param b the resulting string
 * @param base which base to use, for decimal numbers use 10
 * @param plus_sign_if_needed whether to add a plus sign ('+')
 * @param space_sign_if_needed whether to add a space sign (' ')
 * @param padding_no how padding to prefix the string with
 * @param justify where to add the padding, when true, the padding will be added to the end, otherwise it will be added at the front
 * @param zero_pad whether to use zeroes as padding or spaces, true if padding should be zeroes
 * @return the result array
 */
char* pra_itoa_advanced(intmax_t i, char b[], int base, bool plus_sign_if_needed, bool space_sign_if_needed, int padding_no, bool justify, bool zero_pad);
