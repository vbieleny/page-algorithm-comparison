#pragma once

#include <attrs.h>
#include <types.h>
#include <idt.h>

NORETURN INTERRUPT void kernel_panic(interrupt_frame_t* frame, u32 error_code);
NORETURN NO_CALLER_SAVED_REGISTERS void halt();
