#pragma once

#include <idt.h>
#include <attrs.h>

INTERRUPT void pfh_second_isr(interrupt_frame_t* frame, unsigned int error_code);
