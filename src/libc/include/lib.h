#pragma once

#include <types.h>
#include <stddef.h>

void memory_set(void *memory, u8 value, size_t length);
void* memory_copy(void *destination, const void *source, size_t length);
