#pragma once

#include <stdbool.h>

#define PORT 0x3f8          // COM1

bool serial_init();
bool serial_received();
char serial_read();
bool serial_is_transmit_empty();
void serial_putchar(char c);
