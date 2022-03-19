#pragma once

#define UNUSED __attribute__((unused))
#define NORETURN __attribute__((noreturn))
#define INTERRUPT __attribute__((interrupt))
#define NO_CALLER_SAVED_REGISTERS __attribute__((no_caller_saved_registers))
#define SECTION(section_name) __attribute__((section(section_name)))