#ifndef REVERSALS_H
#define REVERSALS_H

#include <stdbool.h>

/**
 * @file reversals.h
 * @brief Utilities related to digit reversal, palindromes, and mirrored numbers.
 *
 * All functions assume input strings contain only verified numerical characters (0-9).
**/


/**
 * @brief Checks whether a numeric string is a palindrome.
 *
 * @param number
 *      Null-terminated numeric string (may contain leading zeroes).
 *
 * @return
 *      - true if the string is a valid palindrome
 *      - false otherwise
 *
 * @note
 *      - Leading zeroes are ignored
 *      - Trailing zeroes invalidate palindromes
 *      - Single-digit numbers are not considered palindromes
 */
bool is_palindrome(const char *number);


/**
 * @brief Generates the next palindrome number greater than or equal to the given number.
 *
 * @param number
 *      Null-terminated numeric string (may contain leading zeroes).
 *
 * @return
 *      - Heap-allocated palindrome string (caller must free)
 *      - NULL on error
 */
char *next_palindrome(const char *number);


/**
 * @brief Generates the next mirror number for a given input.
 *
 * @param number
 *      Numeric string.
 * @param mode
 *      Input mode:
 *        - 'm' -> input is already mirrored
 *        - 'n' -> input is a normal number
 *
 * @return
 *      - Heap-allocated mirrored number string (caller must free)
 *      - NULL on error
 *
 * @note
 *      - In mirrored mode ('m'), trailing zeroes are illegal due to ambiguity
 *      - In normal mode ('n'), leading zeroes are ignored
 */
char *next_mirror_number(char *number, char mode);


#endif /* REVERSALS_H */