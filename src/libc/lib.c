#include <lib.h>
#include <stdint.h>

void* memory_move(void *destination, const void *source, size_t length)
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

void* memory_move_debug(void *destination, const void *source, size_t length)
{
    uint8_t *dest = destination;
    const uint8_t *src = source;
    for (size_t i = 0; i < length; i++)
        dest[i] = src[i];
    return destination;
}
