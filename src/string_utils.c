#include <string_utils.h>

size_t strlen(const char *string)
{
	size_t length = 0;
	while (string[length])
		length++;
	return length;
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
