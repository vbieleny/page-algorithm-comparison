/**
 * @file
 * @brief Simple random number generator.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Generates and returns a random number between 0 and INT_MAX.
 * Uses a very simple LCG (linear congruential generator).
 * @return random number between 0 and INT_MAX
 */
uint32_t pra_rand();

/**
 * @brief Generates and returns a random number in specified range.
 * @param min minimum number to return
 * @param max maximum number to return
 * @return random number between specified min and max
 */
uint32_t pra_rand_between(uint32_t min, uint32_t max);

/**
 * @brief Sets a seed for the random number generator.
 * @param seed what seed to use for generating random numbers
 */
void pra_srand(uint32_t seed);

/**
 * @brief Fills an array with random numbers from a range.
 * @param array an array to fill with random numbers
 * @param size size of the array to fill
 * @param min minimum random number to generate
 * @param max maximum random number to generate
 */
void pra_fill_random(int *array, size_t size, uint32_t min, uint32_t max);
