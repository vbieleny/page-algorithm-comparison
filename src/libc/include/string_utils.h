#pragma once

#include <stddef.h>
#include <stdint.h>

size_t string_length(const char *str);
char* string_int_to_string(int32_t value, char *result, int32_t base);
