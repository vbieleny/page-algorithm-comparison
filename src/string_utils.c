#include <string_utils.h>
#include <lib.h>

size_t strlen(const char *string)
{
	size_t length = 0;
	while (string[length])
		length++;
	return length;
}

char* strcpy(char *destination, const char *source)
{
    size_t length = strlen(source);
    for (size_t i = 0; i < length; i++)
        destination[i] = source[i];
    return destination;
}

char *strcat(char *destination, const char *source)
{
    size_t destination_length = strlen(destination);
    size_t i = 0;
    while (source[i])
    {
        destination[destination_length + i] = source[i];
        i++;
    }
    return destination;
}

char* itoa(int32_t value, char *result, int32_t base)
{
    if (base < 2 || base > 36)
    {
        *result = '\0';
        return result;
    }

    char *ptr = result, *ptr1 = result, tmp_char;
    int32_t tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    if (tmp_value < 0)
        *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

char* itoa_advanced(intmax_t i, char b[], int base, bool plus_sign_if_needed, bool space_sign_if_needed, int padding_no, bool justify, bool zero_pad)
{
    char digit[32] = {0};
    memset(digit, 0, 32);
    strcpy(digit, "0123456789");

    if (base == 16)
    {
        strcat(digit, "ABCDEF");
    }
    else if (base == 17)
    {
        strcat(digit, "abcdef");
        base = 16;
    }

    char *p = b;
    if (i < 0)
    {
        *p++ = '-';
        i *= -1;
    }
    else if (plus_sign_if_needed)
    {
        *p++ = '+';
    }
    else if (!plus_sign_if_needed && space_sign_if_needed)
    {
        *p++ = ' ';
    }

    intmax_t shifter = i;
    do
    {
        ++p;
        shifter = shifter / base;
    } while (shifter);

    *p = '\0';
    do
    {
        *--p = digit[i % base];
        i = i / base;
    } while (i);

    int padding = padding_no - (int) strlen(b);
    if (padding < 0)
        padding = 0;

    if (justify)
    {
        while (padding--)
        {
            if (zero_pad)
            {
                b[strlen(b)] = '0';
            }
            else
            {
                b[strlen(b)] = ' ';
            }
        }
    }
    else
    {
        char a[256] = {0};
        while (padding--)
        {
            if (zero_pad)
            {
                a[strlen(a)] = '0';
            }
            else
            {
                a[strlen(a)] = ' ';
            }
        }
        strcat(a, b);
        strcpy(b, a);
    }

    return b;
}
