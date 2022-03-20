#pragma once

#include <attrs.h>
#include <stdint.h>
#include <idt.h>

NORETURN INTERRUPT void kernel_panic(interrupt_frame_t* frame, uint32_t error_code);
NORETURN NO_CALLER_SAVED_REGISTERS void halt();
