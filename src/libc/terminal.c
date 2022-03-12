#include <lib.h>
#include <string.h>
#include <terminal.h>

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
		for (size_t x = 0; x < VGA_WIDTH; x++)
        {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_move_cursor(size_t x, size_t y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_scroll()
{
    memmove(terminal_buffer, terminal_buffer + VGA_WIDTH, VGA_WIDTH * 2 * (VGA_HEIGHT - 1));
    for (size_t i = 0; i < VGA_WIDTH; i++)
    {
        terminal_putentryat(' ', terminal_color, i, VGA_HEIGHT - 1);
    }
}

void terminal_newline(void)
{
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
    {
        terminal_row = VGA_HEIGHT - 1;
        terminal_scroll();
    }
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
    if (c == '\n')
    {
        terminal_newline();
    }
    else
    {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH)
            terminal_newline();
    }
    terminal_move_cursor(terminal_column, terminal_row);
}

void terminal_write(const char *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char *data)
{
	terminal_write(data, strlen(data));
}

void terminal_printf(const char *format, ...)
{
    char buffer[11];
    va_list list;
    va_start(list, format);
    while (*format)
    {
        if (*format != '%')
        {
            terminal_putchar(*format);
            format++;
            continue;
        }
        format++;
        switch (*format)
        {
            case 's':
                terminal_writestring(va_arg(list, char *));
                break;
            case 'd':
                itoa(va_arg(list, int), buffer, 10);
                terminal_writestring(buffer);
                break;
            case 'x':
                itoa(va_arg(list, int), buffer, 16);
                terminal_writestring(buffer);
        }
        format++;
    }
    va_end(list);
}
