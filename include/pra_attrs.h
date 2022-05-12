/**
 * @file
 * @brief Macro definitions for commonly used attributes.
 */

#pragma once

#define PRA_UNUSED __attribute__((unused))
#define PRA_NORETURN __attribute__((noreturn))
#define PRA_PACKED __attribute__((packed))
#define PRA_ALWAYS_INLINE __attribute__((always_inline)) inline
#define PRA_ALIGNED(alignment) __attribute__((aligned(alignment)))
#define PRA_SECTION(section_name) __attribute__((section(section_name)))
