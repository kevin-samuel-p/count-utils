#ifndef REPS_H
#define REPS_H

#include <stdbool.h>

/**
 * @file reps.h
 * @brief Utilities for detecting and generating numbers based on digit repetition patterns.
 *
 * Provides functions for:
 *  - Detecting repeating digits in a number
 *  - Generating the next number with or without repeating digits
**/


/**
 * @brief Checks whether a number contains any repeating digits.
 *
 * @param number
 *      Non-negative integer to check.
 *
 * @return
 *      - true if at least one digit repeats
 *      - false if all digits are unique
 *
 * @note
 *      Any number greater than 9876543210 is guaranteed to have repeating digits
 *      (pigeonhole principle).
 */
bool is_repeating(unsigned long long number);


/**
 * @brief Returns the next number greater than the input that contains at least one repeating digit.
 *
 * @param number
 *      Non-negative integer to start from.
 *
 * @return
 *      - Next number containing repeating digits
 *      - 0 on error (e.g., if input == ULLONG_MAX)
 */
unsigned long long next_repeating_number(unsigned long long number);


/**
 * @brief Returns the next number greater than the input that contains no repeating digits.
 *
 * @param number
 *      Non-negative integer to start from.
 *
 * @return
 *      - Next non-repeating number
 *      - -1 on failure
 *
 * @note
 *      - Maximum supported value is 9876543210
 *      - Uses a lexicographic depth-first search internally
 */
long long next_non_repeating_number(unsigned long long number);

#endif /* REPS_H */