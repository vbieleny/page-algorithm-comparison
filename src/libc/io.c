#include <io.h>

inline void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %1, %0" : : "a"(value), "Nd"(port));
}

inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %0, %1" : "=a"(ret) : "Nd"(port));
    return ret;
}
