#pragma once

#include <stddef.h>

void* memmove(void *destination, const void *source, size_t length);
void* memmove_debug(void *destination, const void *source, size_t length);
void* rmalloc(size_t size);
