#include <stdint.h>
#include <lib.h>

void *memmove(void *destination, const void *source, size_t length)
{
    uint8_t *dest = destination;
    const uint8_t *src = source;
    for (size_t i = 0; i < length; i++)
        dest[i] = src[i];
    return destination;
}
