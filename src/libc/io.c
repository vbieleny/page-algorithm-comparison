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

/**
 * Wait a very small amount of time (1 to 4 microseconds, generally)
 */
inline void io_wait()
{
    outb(0x80, 0);
}

inline void invalidate_page(uint32_t address) {
   asm volatile("invlpg [%0]" ::"r" (address) : "memory");
}
