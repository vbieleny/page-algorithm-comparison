#include "pra_io.h"
#include "pra_lib.h"
#include "pra_terminal.h"
#include "pra_serial.h"
#include "pra_string.h"

static io_stream_e current_stream = DEFAULT_STREAM;

static void display_character(io_stream_e stream, char c, int *a)
{
    io_sputchar(stream, c);
    *a += 1;
}

static void display_string(io_stream_e stream, char *c, int *a)
{
    for (int i = 0; c[i]; ++i)
        display_character(stream, c[i], a);
}

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
    io_swrite(stream, str, pra_strlen(str));
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

int io_printf(const char *format, ...)
{
    va_list list;
    va_start(list, format);
    int i = io_vprintf(current_stream, format, list);
    va_end(list);
    return i;
}

int io_vprintf(io_stream_e stream, const char *format, va_list list)
{
    int chars = 0;
    char int_str_buffer[256] = {0};

    for (int i = 0; format[i]; ++i)
    {
        char specifier = '\0';
        char length = '\0';

        int length_spec = 0;
        int prec_spec = 0;
        bool percent_escape = false;
        bool left_justify = false;
        bool zero_pad = false;
        bool space_no_sign = false;
        bool alt_form = false;
        bool plus_sign = false;
        bool emode = false;
        int expo = 0;

        if (format[i] == '%')
        {
            ++i;

            bool ext_break = false;
            while (1)
            {

                switch (format[i])
                {
                    case '-':
                        left_justify = true;
                        ++i;
                        break;

                    case '+':
                        plus_sign = true;
                        ++i;
                        break;

                    case '#':
                        alt_form = true;
                        ++i;
                        break;

                    case ' ':
                        space_no_sign = true;
                        ++i;
                        break;

                    case '0':
                        zero_pad = true;
                        ++i;
                        break;

                    case '%':
                        percent_escape = true;
                        ext_break = true;
                        break;

                    default:
                        ext_break = true;
                        break;
                }

                if (ext_break)
                    break;
            }

            if (percent_escape)
            {
                display_character(stream, '%', &chars);
                continue;
            }

            while (pra_isdigit(format[i]))
            {
                length_spec *= 10;
                length_spec += format[i] - 48;
                ++i;
            }

            if (format[i] == '*')
            {
                length_spec = va_arg(list, int);
                ++i;
            }

            if (format[i] == '.')
            {
                ++i;
                while (pra_isdigit(format[i]))
                {
                    prec_spec *= 10;
                    prec_spec += format[i] - 48;
                    ++i;
                }

                if (format[i] == '*')
                {
                    prec_spec = va_arg(list, int);
                    ++i;
                }
            }
            else
            {
                prec_spec = 6;
            }

            if (format[i] == 'h' || format[i] == 'l' || format[i] == 'j' ||
                format[i] == 'z' || format[i] == 't' || format[i] == 'L')
            {
                length = format[i];
                ++i;
                if (format[i] == 'h')
                {
                    length = 'H';
                }
                else if (format[i] == 'l')
                {
                    length = 'q';
                    ++i;
                }
            }
            specifier = format[i];

            pra_memset(int_str_buffer, 0, 256);

            int base = 10;
            if (specifier == 'o')
            {
                base = 8;
                specifier = 'u';
                if (alt_form)
                {
                    display_string(stream, "0", &chars);
                }
            }
            if (specifier == 'p')
            {
                base = 16;
                length = 'z';
                specifier = 'u';
            }
            switch (specifier)
            {
                case 'X':
                    base = 16;
                case 'x':
                    base = base == 10 ? 17 : base;
                    if (alt_form)
                    {
                        display_string(stream, "0x", &chars);
                    }

                case 'u':
                {
                    switch (length)
                    {
                        case 0:
                        {
                            unsigned int integer = va_arg(list, unsigned int);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'H':
                        {
                            unsigned char integer = (unsigned char) va_arg(list, unsigned int);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'h':
                        {
                            unsigned short int integer = va_arg(list, unsigned int);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'l':
                        {
                            unsigned long integer = va_arg(list, unsigned long);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'q':
                        {
                            unsigned long long integer = va_arg(list, unsigned long long);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'j':
                        {
                            uintmax_t integer = va_arg(list, uintmax_t);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'z':
                        {
                            size_t integer = va_arg(list, size_t);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 't':
                        {
                            ptrdiff_t integer = va_arg(list, ptrdiff_t);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }

                case 'd':
                case 'i':
                {
                    switch (length)
                    {
                        case 0:
                        {
                            int integer = va_arg(list, int);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'H':
                        {
                            signed char integer = (signed char) va_arg(list, int);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'h':
                        {
                            short int integer = va_arg(list, int);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'l':
                        {
                            long integer = va_arg(list, long);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'q':
                        {
                            long long integer = va_arg(list, long long);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'j':
                        {
                            intmax_t integer = va_arg(list, intmax_t);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 'z':
                        {
                            size_t integer = va_arg(list, size_t);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        case 't':
                        {
                            ptrdiff_t integer = va_arg(list, ptrdiff_t);
                            pra_itoa_advanced(integer, int_str_buffer, base, plus_sign, space_no_sign, length_spec, left_justify, zero_pad);
                            display_string(stream, int_str_buffer, &chars);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }

                case 'c':
                {
                    if (length == 'l')
                    {
                        display_character(stream, va_arg(list, unsigned int), &chars);
                    }
                    else
                    {
                        display_character(stream, va_arg(list, int), &chars);
                    }

                    break;
                }

                case 's':
                {
                    display_string(stream, va_arg(list, char *), &chars);
                    break;
                }

                case 'n':
                {
                    switch (length)
                    {
                        case 'H':
                            *(va_arg(list, signed char *)) = chars;
                            break;
                        case 'h':
                            *(va_arg(list, short int *)) = chars;
                            break;

                        case 0:
                        {
                            int *a = va_arg(list, int *);
                            *a = chars;
                            break;
                        }

                        case 'l':
                            *(va_arg(list, long *)) = chars;
                            break;
                        case 'q':
                            *(va_arg(list, long long *)) = chars;
                            break;
                        case 'j':
                            *(va_arg(list, intmax_t *)) = chars;
                            break;
                        case 'z':
                            *(va_arg(list, size_t *)) = chars;
                            break;
                        case 't':
                            *(va_arg(list, ptrdiff_t *)) = chars;
                            break;
                        default:
                            break;
                    }
                    break;
                }

                case 'e':
                case 'E':
                    emode = true;

                case 'f':
                case 'F':
                case 'g':
                case 'G':
                {
                    double floating = va_arg(list, double);

                    while (emode && floating >= 10)
                    {
                        floating /= 10;
                        ++expo;
                    }

                    int form = length_spec - prec_spec - expo - (prec_spec || alt_form ? 1 : 0);
                    if (emode)
                    {
                        form -= 4;  // For 'e+00'
                    }
                    if (form < 0)
                    {
                        form = 0;
                    }

                    pra_itoa_advanced(floating, int_str_buffer, base, plus_sign, space_no_sign, form,
                              left_justify, zero_pad);

                    display_string(stream, int_str_buffer, &chars);

                    floating -= (int) floating;

                    for (int i = 0; i < prec_spec; ++i)
                    {
                        floating *= 10;
                    }
                    intmax_t decPlaces = (intmax_t) (floating + 0.5);

                    if (prec_spec)
                    {
                        display_character(stream, '.', &chars);
                        pra_itoa_advanced(decPlaces, int_str_buffer, 10, false, false, 0, false, false);
                        int_str_buffer[prec_spec] = 0;
                        display_string(stream, int_str_buffer, &chars);
                    }
                    else if (alt_form)
                    {
                        display_character(stream, '.', &chars);
                    }

                    break;
                }

                case 'a':
                case 'A':
                    // TODO: Hexadecimal floating points
                    break;

                default:
                    break;
            }

            if (specifier == 'e')
            {
                display_string(stream, "e+", &chars);
            }
            else if (specifier == 'E')
            {
                display_string(stream, "E+", &chars);
            }

            if (specifier == 'e' || specifier == 'E')
            {
                pra_itoa_advanced(expo, int_str_buffer, 10, false, false, 2, false, true);
                display_string(stream, int_str_buffer, &chars);
            }
        }
        else
        {
            display_character(stream, format[i], &chars);
        }
    }

    return chars;
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
