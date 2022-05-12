/**
 * @file
 * @brief Header for handling input and output operations.
 */

#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    IO_TERMINAL,
    IO_SERIAL,
    IO_NONE
} io_stream_e;

#ifndef DEFAULT_STREAM
#define DEFAULT_STREAM IO_TERMINAL
#endif

/**
 * @brief Sets the current IO stream, which will be used by all IO functions.
 * @param stream which stream to use
 */
void io_set_stream(io_stream_e stream);

/**
 * @brief Returns the current stream.
 * @return currently set stream
 */
io_stream_e io_get_stream();

/**
 * @brief Outputs one character to the specified stream.
 * @param stream which stream to output to
 * @param c character that will be outputted
 */
void io_sputchar(io_stream_e stream, char c);

/**
 * @brief Writes a string of some length to the specified stream.
 * @param stream which stream to output to
 * @param str string which will be outputted
 * @param length the length of the string
 */
void io_swrite(io_stream_e stream, const char *str, size_t length);

/**
 * @brief Writes a string to the specified stream.
 * @param stream which stream to output to
 * @param str string which will be outputted
 */
void io_swritestring(io_stream_e stream, const char *str);

void io_sprintf(io_stream_e stream, const char *format, ...);

void io_putchar(char c);
void io_write(const char *str, size_t length);
void io_writestring(const char *str);
__attribute__((format(printf, 1, 2))) int io_printf(const char *format, ...);

int io_vprintf(io_stream_e stream, const char *format, va_list arguments);

void io_out_byte(uint16_t port, uint8_t value);
uint8_t io_in_byte(uint16_t port);
void io_out_word(uint16_t port, uint16_t value);
uint16_t io_in_word(uint16_t port);

void io_wait();
void io_shutdown();
