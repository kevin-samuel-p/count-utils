#ifndef INCREASING_H
#define INCREASING_H

#include <stdbool.h>

/**
 * @file increasing.h
 * @brief Utilities for working with numbers whose digits are in non-decreasing order.
 *
 * Provides validation and generation of numbers whose digits
 * appear in non-decreasing (increasing) order.
**/


/**
 * @brief Checks whether the digits of a numerical string are in non-decreasing order.
 *
 * @param number
 *      Null-terminated numerical string to check.
 *
 * @pre
 *      - @p number contains digits only ('0'–'9')
 *      - No negative sign
 *
 * @return
 *      - true  if digits are in non-decreasing order
 *      - false otherwise
 */
bool has_increasing_digits(const char *number);


/**
 * @brief Generates the next number whose digits are in non-decreasing order.
 *
 * Given a validated numerical string whose digits are already
 * in non-decreasing order, this function returns the next
 * such number in lexicographic order.
 *
 * @param number
 *      Null-terminated numerical string with non-decreasing digits.
 *
 * @pre
 *      - @p number is already validated
 *      - Digits are in non-decreasing order
 *      - No negative sign
 *
 * @return
 *      - Heap-allocated numerical string on success
 *      - NULL on allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *next_increasing_number(const char *number);


#endif /* INCREASING_H */