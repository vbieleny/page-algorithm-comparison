#include <io.h>
#include <terminal.h>
#include <serial.h>
#include <string_utils.h>

static io_stream_e current_stream = DEFAULT_STREAM;

void io_set_stream(io_stream_e stream)
{
    current_stream = stream;
}

io_stream_e io_get_stream()
{
    return current_stream;
}

void io_sputchar(io_stream_e stream, char c)
{
    switch (stream)
    {
    case IO_TERMINAL:
        terminal_putchar(c);
        break;
    case IO_SERIAL:
        serial_putchar(c);
        break;
    case IO_NONE:
        // Do nothing
        break;
    }
}

void io_swrite(io_stream_e stream, const char *str, size_t length)
{
    for (size_t i = 0; i < length; i++)
        io_sputchar(stream, str[i]);
}

void io_swritestring(io_stream_e stream, const char *str)
{
    io_swrite(stream, str, strlen(str));
}

void io_sprintf(io_stream_e stream, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    io_vprintf(stream, format, arguments);
    va_end(arguments);
}

void io_putchar(char c)
{
    io_sputchar(current_stream, c);
}

void io_write(const char *str, size_t length)
{
    io_swrite(current_stream, str, length);
}

void io_writestring(const char *str)
{
    io_swritestring(current_stream, str);
}

void io_printf(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    io_vprintf(current_stream, format, arguments);
    va_end(arguments);
}

void io_vprintf(io_stream_e stream, const char *format, va_list arguments)
{
    char buffer[11];
    while (*format)
    {
        if (*format != '%')
        {
            io_sputchar(stream, *format);
            format++;
            continue;
        }
        format++;
        switch (*format)
        {
        case 'c':
            io_sputchar(stream, va_arg(arguments, int));
            break;
        case 's':
            io_swritestring(stream, va_arg(arguments, char *));
            break;
        case 'd':
            itoa(va_arg(arguments, int), buffer, 10);
            io_swritestring(stream, buffer);
            break;
        case 'x':
            itoa(va_arg(arguments, int), buffer, 16);
            io_swritestring(stream, buffer);
            break;
        case '%':
            io_sputchar(stream, '%');
            break;
        }
        format++;
    }
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

void io_out_word(uint16_t port, uint16_t value)
{
    asm volatile("outw %1, %0" : : "a"(value), "Nd"(port));
}

uint16_t io_in_word(uint16_t port)
{
    uint16_t value;
    asm volatile("inw %0, %1" : "=a"(value) : "Nd"(port));
    return value;
}

/**
 * Wait a very small amount of time (1 to 4 microseconds, generally)
 */
void io_wait()
{
    io_out_byte(0x80, 0);
}

void io_shutdown()
{
    io_out_word(0xb004, 0x2000);    // Bochs and QEMU < 2.0
    io_out_word(0x604, 0x2000);     // QEMU
    io_out_word(0x4004, 0x3400);    // VirtualBox
}
