#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

size_t pra_strlen(const char *str);
char* pra_strcpy(char *destination, const char *source);
char* pra_strcat(char *destination, const char *source);
char* pra_itoa(int32_t value, char *result, int32_t base);
char* pra_itoa_advanced(intmax_t i, char b[], int base, bool plus_sign_if_needed, bool space_sign_if_needed, int padding_no, bool justify, bool zero_pad);
