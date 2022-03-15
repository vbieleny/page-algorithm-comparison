#pragma once

#include <idt.h>

__attribute__((interrupt))
void pfh_second_isr(interrupt_frame_t* frame, unsigned int error_code);
