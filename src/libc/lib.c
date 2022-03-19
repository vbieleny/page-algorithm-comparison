#include <lib.h>

void memory_set(void *memory, u8 value, size_t length)
{
    u8 *destination_memory = (u8*) memory;
    for (size_t i = 0; i < length; i++)
        destination_memory[i] = value;
}

void* memory_copy(void *destination, const void *source, size_t length)
{
    asm volatile("rep movsb"
                : "=D" (destination),
                  "=S" (source),
                  "=c" (length)
                : "0" (destination),
                  "1" (source),
                  "2" (length)
                : "memory");
    return destination;
}
