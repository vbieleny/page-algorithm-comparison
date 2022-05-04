#pragma once

#include <stdint.h>
#include <idt.h>

#define TIMER_FREQUENCY (3579545 / 3)
#define TIMER_DIVISOR_1KHZ 1193

void timer_initialize();
void timer_set_divisor(uint16_t divisor);
uint32_t milliseconds_from_boot();
void pra_timer_interrupt();

ALWAYS_INLINE uint64_t timestamp()
{
    uint64_t value;
    asm volatile("lfence\t\n"
                 "rdtsc\t\n" : "=A"(value));
    return value;
}

ALWAYS_INLINE uint64_t timestamp_scaled()
{
    uint64_t value;
    asm volatile("lfence\t\n"
                 "rdtsc\t\n" : "=A"(value));
    return value / 100ULL;
}
