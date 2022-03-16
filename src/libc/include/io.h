#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

typedef enum
{
    IO_TERMINAL, IO_SERIAL
} io_stream;

#define DEFAULT_STREAM IO_TERMINAL

void io_putchar(io_stream stream, char c);
void io_write(io_stream stream, const char *str, size_t length);
void io_writestring(io_stream stream, const char *str);
void io_printf(io_stream stream, const char *format, ...);
void io_out_byte(uint16_t port, uint8_t value);
void io_out_word(uint16_t port, uint16_t value);
uint8_t io_in_byte(uint16_t port);
uint16_t io_in_word(uint16_t port);
void io_wait();
void io_shutdown();
