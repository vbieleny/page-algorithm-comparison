#pragma once

#include <types.h>
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
void io_out_byte(u16 port, u8 value);
void io_out_word(u16 port, u16 value);
u8 io_in_byte(u16 port);
u16 io_in_word(u16 port);
void io_wait();
void io_shutdown();
