#include <timer.h>
#include <pic.h>
#include <io.h>

static uint32_t timer_counter;

void timer_initialize()
{
    idt_set_descriptor(32, &timer_interrupt, 0x8e);
}

void timer_interrupt(interrupt_frame_t *frame)
{
    timer_counter++;
}

void timer_set_divisor(uint16_t divisor)
{
    io_out_byte(0x43, 0x36);
    io_out_byte(0x40, divisor & 0xff);
    io_out_byte(0x40, divisor >> 8);
}

uint32_t milliseconds_from_boot()
{
    return timer_counter;
}

inline uint64_t rdtsc()
{
    uint64_t ret;
    asm volatile("lfence\t\n"
                 "rdtsc\t\n" : "=A"(ret));
    return ret;
}
