/**
 * @file
 * @brief Hardware timer functionality.
 */

#pragma once

#include <stdint.h>
#include "pra_idt.h"

/**
 * @brief Approximation of the base hardware timer frequency.
 */
#define TIMER_FREQUENCY (3579545 / 3)

/**
 * @brief The value of divisor for 1 KHZ frequency.
 */
#define TIMER_DIVISOR_1KHZ 1193

/**
 * @brief Function that get called when timer interupt occurs.
 */
typedef void (*timer_callback_t)();

/**
 * @brief Initializes the timer functionality. Should be called only once.
 */
void timer_initialize();

/**
 * @brief Sets the frequency divisor for the hardware timer.
 * The base frequency will be divided by the divisor and the resulting frequency will be used for interrups.
 * @param divisor the divisor to use
 */
void timer_set_divisor(uint16_t divisor);

/**
 * @brief Sets the timer callback that will be executed when a timer interrupt occurs.
 * @param callback the callback that will be executed when an interrupt occurs
 */
void timer_set_callback(timer_callback_t callback);

/**
 * @brief General timer interrupt handler that will call the set callback and send an EOI to the PIC.
 * Should not be called manually.
 */
void pra_timer_interrupt();

/**
 * @brief Returns the current timestamp. The returned value is number of instructions executed overall.
 * @return the number of instructions executed
 */
PRA_ALWAYS_INLINE uint64_t timestamp()
{
    uint64_t value;
    asm volatile("lfence\t\n"
                 "rdtsc\t\n" : "=A"(value));
    return value;
}

/**
 * @brief Returns the current scaled timestamp. The returned value is number of instructions executed overall divided by certain value.
 * @return the number of instructions executed scaled
 */
PRA_ALWAYS_INLINE uint64_t timestamp_scaled()
{
    uint64_t value;
    asm volatile("lfence\t\n"
                 "rdtsc\t\n" : "=A"(value));
    return value / 100ULL;
}
