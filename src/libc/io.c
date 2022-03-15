#include <io.h>

void io_out_byte(uint16_t port, uint8_t value)
{
    asm volatile("outb %1, %0" : : "a"(value), "Nd"(port));
}

uint8_t io_in_byte(uint16_t port)
{
    uint8_t value;
    asm volatile("inb %0, %1" : "=a"(value) : "Nd"(port));
    return value;
}

/**
 * Wait a very small amount of time (1 to 4 microseconds, generally)
 */
void io_wait()
{
    io_out_byte(0x80, 0);
}
