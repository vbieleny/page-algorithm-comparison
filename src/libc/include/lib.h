#pragma once

#include <stddef.h>

void* memory_move(void *destination, const void *source, size_t length);
void* memory_move_debug(void *destination, const void *source, size_t length);
