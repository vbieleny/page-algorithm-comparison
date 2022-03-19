#pragma once

#include <stddef.h>
#include <types.h>

size_t string_length(const char *str);
char* string_int_to_string(s32 value, char *result, s32 base);
