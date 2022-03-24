#pragma once

#include <stdint.h>
#include <idt.h>

#define TIMER_FREQUENCY (3579545 / 3)
#define TIMER_DIVISOR_1KHZ 1193

void timer_initialize();
void timer_set_divisor(uint16_t divisor);
uint32_t milliseconds_from_boot();
INTERRUPT void timer_interrupt(interrupt_frame_t *frame);

ALWAYS_INLINE uint64_t rdtsc()
{
    uint64_t value;
    asm volatile("lfence\t\n"
                 "rdtsc\t\n" : "=A"(value));
    return value;
}
