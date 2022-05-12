#pragma once

#include <stdint.h>
#include "pra_idt.h"

#define TIMER_FREQUENCY (3579545 / 3)
#define TIMER_DIVISOR_1KHZ 1193

typedef void (*timer_callback_t)();

extern void asm_timer_interrupt();

void timer_initialize();
void timer_set_divisor(uint16_t divisor);
void timer_set_callback(timer_callback_t callback);
void pra_timer_interrupt();

PRA_ALWAYS_INLINE uint64_t timestamp()
{
    uint64_t value;
    asm volatile("lfence\t\n"
                 "rdtsc\t\n" : "=A"(value));
    return value;
}

PRA_ALWAYS_INLINE uint64_t timestamp_scaled()
{
    uint64_t value;
    asm volatile("lfence\t\n"
                 "rdtsc\t\n" : "=A"(value));
    return value / 100ULL;
}
