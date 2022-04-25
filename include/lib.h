#pragma once

#include <stdint.h>
#include <stddef.h>

typedef char prac_name_t[128];

void memory_set(void *memory, uint8_t value, size_t length);
void* memory_copy(void *destination, const void *source, size_t length);
