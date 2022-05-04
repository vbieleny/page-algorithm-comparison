#pragma once

#define UNUSED __attribute__((unused))
#define NORETURN __attribute__((noreturn))
#define PACKED __attribute__((packed))
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define ALIGNED(alignment) __attribute__((aligned(alignment)))
#define SECTION(section_name) __attribute__((section(section_name)))
