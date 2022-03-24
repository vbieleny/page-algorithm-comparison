#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

typedef enum
{
    IO_TERMINAL, IO_SERIAL
} io_stream_e;

#define DEFAULT_STREAM IO_TERMINAL

void io_sputchar(io_stream_e stream, char c);
void io_swrite(io_stream_e stream, const char *str, size_t length);
void io_swritestring(io_stream_e stream, const char *str);
void io_sprintf(io_stream_e stream, const char *format, ...);

void io_putchar(char c);
void io_write(const char *str, size_t length);
void io_writestring(const char *str);
void io_printf(const char *format, ...);

void io_vprintf(io_stream_e stream, const char *format, va_list arguments);

void io_out_byte(uint16_t port, uint8_t value);
uint8_t io_in_byte(uint16_t port);
void io_out_word(uint16_t port, uint16_t value);
uint16_t io_in_word(uint16_t port);

void io_wait();
void io_shutdown();
