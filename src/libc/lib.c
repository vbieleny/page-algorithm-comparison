#include <lib.h>

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
