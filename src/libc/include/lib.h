#pragma once

#include <stdint.h>
#include <stddef.h>

void memory_set(void *memory, uint8_t value, size_t length);
void* memory_move(void *destination, const void *source, size_t length);
