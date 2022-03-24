#pragma once

#define UNUSED __attribute__((unused))
#define NORETURN __attribute__((noreturn))
#define PACKED __attribute__((packed))
#define INTERRUPT __attribute__((interrupt))
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define NO_CALLER_SAVED_REGISTERS __attribute__((no_caller_saved_registers))
#define ALIGNED(alignment) __attribute__((aligned(alignment)))
#define SECTION(section_name) __attribute__((section(section_name)))
