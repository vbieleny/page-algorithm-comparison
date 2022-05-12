#include "pra_io.h"
#include "pra_pic.h"
#include "pra_timer.h"

static timer_callback_t timer_callback;

void timer_initialize()
{
    idt_set_descriptor(32, &asm_timer_interrupt, 0x8e);
}

void pra_timer_interrupt()
{
    if (timer_callback)
        timer_callback();
    pic_send_eoi(0);
}

void timer_set_divisor(uint16_t divisor)
{
    // bool interrupts_enabled = are_interrupts_enabled();
    // set_interrupts_enabled(false);

    io_out_byte(0x43, 0x36);
    io_out_byte(0x40, divisor & 0xff);
    io_out_byte(0x40, divisor >> 8);

    // set_interrupts_enabled(interrupts_enabled);
}

void timer_set_callback(timer_callback_t callback)
{
    timer_callback = callback;
}
