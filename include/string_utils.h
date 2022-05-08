#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

size_t strlen(const char *str);
char* strcpy(char *destination, const char *source);
char* strcat(char *destination, const char *source);
char* itoa(int32_t value, char *result, int32_t base);
char* itoa_advanced(intmax_t i, char b[], int base, bool plus_sign_if_needed, bool space_sign_if_needed, int padding_no, bool justify, bool zero_pad);
