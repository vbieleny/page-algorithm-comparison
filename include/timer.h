#pragma once

#include <stdint.h>
#include <idt.h>

#define TIMER_FREQUENCY (3579545 / 3)
#define TIMER_DIVISOR_991_HZ 1204

void timer_initialize();
INTERRUPT void timer_interrupt(interrupt_frame_t *frame);
void timer_set_divisor(uint16_t divisor);
uint32_t milliseconds_from_boot();

ALWAYS_INLINE uint64_t rdtsc();
