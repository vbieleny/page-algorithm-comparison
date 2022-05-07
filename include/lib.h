#pragma once

#include <stdint.h>
#include <stddef.h>

#define ARRAY_LEN(array) (sizeof((array)) / sizeof((array)[0]))

typedef char prac_name_t[128];

void pra_memset(void *memory, uint8_t value, size_t length);
void* pra_memcpy(void *destination, const void *source, size_t length);
int isdigit(int digit);
