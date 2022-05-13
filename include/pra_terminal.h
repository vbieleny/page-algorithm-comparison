/**
 * @file
 * @brief Provides functionality to write to the VGA video memory.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

/**
 * @brief Enum specifying colors that can be used by the VGA.
 */
typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color_e;

/**
 * @brief Initializes the terminal and clears the screen. This is called automatically by kernel.
 */
void terminal_initialize();

/**
 * @brief Moves cursor at a specified position.
 * @param x horizontal position of the cursor
 * @param y vertical position of the cursor
 */
void terminal_move_cursor(size_t x, size_t y);

/**
 * @brief Sets set active color that will be used to print characters.
 * @param color combination of foreground and background color to use
 */
void terminal_setcolor(uint8_t color);

/**
 * @brief Puts the character with a specified color at a specified position.
 * @param c character to print
 * @param color combination of foreground and background color to use
 * @param x horizonal position where the character will be printed
 * @param y vertical position where the character will be printed
 */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

/**
 * @brief Puts the character with active color at the current cursor position.
 * @param c the character to print
 */
void terminal_putchar(char c);
