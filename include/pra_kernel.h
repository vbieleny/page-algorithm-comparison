/**
 * @file
 * @brief Specifies a kernel function that will be run as soon as bootloader exits.
 */

#pragma once

#include "pra_attrs.h"

/**
 * @brief Main kernel function that will be executed from bootloader.
 */
PRA_UNUSED void kernel_main() PRA_SECTION(".text.kernel");
