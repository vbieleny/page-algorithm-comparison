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

/**
 * @brief Writes a formatted string to the specified stream.
 * @param stream which stream to output to
 * @param format formatted string
 */
void io_sprintf(io_stream_e stream, const char *format, ...);

/**
 * @brief Outputs one character to the default stream.
 * @param c character that will be outputted
 */
void io_putchar(char c);

/**
 * @brief Writes a string of some length to the default stream.
 * @param str string which will be outputted
 * @param length the length of the string
 */
void io_write(const char *str, size_t length);

/**
 * @brief Writes a string to the default stream.
 * @param str string which will be outputted
 */
void io_writestring(const char *str);

/**
 * @brief Writes a formatted string to the default stream.
 * @param format formatted string
 * @return how many characters were outputted
 */
__attribute__((format(printf, 1, 2))) int io_printf(const char *format, ...);

/**
 * @brief Writes a formatted string to the specified stream.
 * @param stream which stream to output to
 * @param format formatted string
 * @param arguments variables for interpolation in formatted string
 * @return how many characters were outputted
 */
int io_vprintf(io_stream_e stream, const char *format, va_list arguments);

/**
 * @brief Writes a byte to a port.
 * @param port port which to output
 * @param value value which will be written to port
 */
void io_out_byte(uint16_t port, uint8_t value);

/**
 * @brief Read a byte from a port.
 * @param port from which port to read
 * @return read value from the port
 */
uint8_t io_in_byte(uint16_t port);

/**
 * @brief Writes a word to a port.
 * @param port port which to output
 * @param value value which will be written to port
 */
void io_out_word(uint16_t port, uint16_t value);

/**
 * @brief Read a word from a port.
 * @param port from which port to read
 * @return read value from the port
 */
uint16_t io_in_word(uint16_t port);

/**
 * @brief Waits a very small amount of time (1 to 4 microseconds, generally).
 */
void io_wait();

/**
 * @brief Shutdowns the computer. Currently works only for emulators (QEMU and Bochs).
 */
void io_shutdown();
