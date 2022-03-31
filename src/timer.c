#include <io.h>
#include <pic.h>
#include <timer.h>

static volatile uint32_t timer_counter;

void timer_initialize()
{
    idt_set_descriptor(32, &timer_interrupt, 0x8e);
}

void timer_interrupt(interrupt_frame_t *frame)
{
    timer_counter++;
    pic_send_eoi(0);
}

void timer_set_divisor(uint16_t divisor)
{
    // TODO: Disable interrupts while communicating with PIT
    io_out_byte(0x43, 0x36);
    io_out_byte(0x40, divisor & 0xff);
    io_out_byte(0x40, divisor >> 8);
}

uint32_t milliseconds_from_boot()
{
    return timer_counter;
}
