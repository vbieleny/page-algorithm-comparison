#include <io.h>
#include <terminal.h>
#include <serial.h>
#include <string_utils.h>

void io_putchar(io_stream stream, char c)
{
    switch (stream)
    {
    case IO_TERMINAL:
        terminal_putchar(c);
        break;
    case IO_SERIAL:
        serial_putchar(c);
        break;
    }
}

void io_write(io_stream stream, const char *str, size_t length)
{
    for (size_t i = 0; i < length; i++)
        io_putchar(stream, str[i]);
}

void io_writestring(io_stream stream, const char *str)
{
    io_write(stream, str, string_length(str));
}

void io_printf(io_stream stream, const char *format, ...)
{
    char buffer[11];
    va_list list;
    va_start(list, format);
    while (*format)
    {
        if (*format != '%')
        {
            io_putchar(stream, *format);
            format++;
            continue;
        }
        format++;
        switch (*format)
        {
        case 's':
            io_writestring(stream, va_arg(list, char *));
            break;
        case 'd':
            string_int_to_string(va_arg(list, int), buffer, 10);
            io_writestring(stream, buffer);
            break;
        case 'x':
            string_int_to_string(va_arg(list, int), buffer, 16);
            io_writestring(stream, buffer);
        }
        format++;
    }
    va_end(list);
}

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
