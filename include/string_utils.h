#pragma once

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);
char* itoa(int32_t value, char *result, int32_t base);
char* strcpy(char *destination, const char *source);
