/**
 * @file
 * @brief Functionality for communicating with serial ports.
 */

#pragma once

#include <stdbool.h>

/**
 * @brief COM1 port.
 */
#define PORT 0x3f8

/**
 * @brief Initializes the serial communication and tests the connection to the serial port.
 * @return true if the initialization was successfull, otherwise false
 */
bool serial_initialize();

/**
 * @brief Returns whether we received a byte from serial port.
 * @return true if there is byte to be read from serial port, otherwise false
 */
bool serial_received();

/**
 * @brief Returns the received byte from a serial port. If there is no byte, it waits until it received one.
 * @return character read from a serial port.
 */
char serial_read();

/**
 * @brief Check whether the transmit it empty and can send data to serial port.
 * @return true if transmit is empty, otherwise false
 */
bool serial_is_transmit_empty();

/**
 * @brief Writes a character to the serial port. Waits until the transmit is empty if necessary.
 */
void serial_putchar(char c);
