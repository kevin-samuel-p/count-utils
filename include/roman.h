#ifndef ROMAN_H
#define ROMAN_H

#include <wchar.h>

/**
 * @file roman.h
 * @brief Utilities for converting between numerical strings and Roman numerals.
 *
 * Supported features:
 *  - Standard Roman numerals (I, V, X, L, C, D, M)
 *  - Extended Roman numerals using vinculum (overline) notation
 *  - Optional idiomatic representation (e.g., replacing I̅ with M)
 *
 * @note
 *  - All numerical input must be:
 *      * Null-terminated
 *      * Non-negative
 *      * Stripped of leading zeroes before calling
 *  - All returned strings are heap-allocated and must be freed by the caller
**/


/**
 * @brief Converts a Roman numeral string into a numeric string.
 *
 * @param roman
 *      Null-terminated wide-character Roman numeral string.
 *      Must contain only valid Roman symbols and vinculum notation.
 *
 * @return
 *      Heap-allocated numeric string on success.
 *      NULL on invalid input or allocation failure.
 */
char *roman_to_number(const wchar_t *roman);


/**
 * @brief Converts a numeric string into a Roman numeral string.
 *
 * @param number
 *      Null-terminated numeric string representing a positive integer.
 *
 * @param representation
 *      Representation mode:
 *          - 'i' : idiomatic (replace I̅ with M)
 *          - 'l' : legal canonical vinculum form
 *
 * @return
 *      Heap-allocated wide-character Roman numeral string on success.
 *      NULL on invalid input or allocation failure.
 */
wchar_t *number_to_roman(const char *number, char representation);


#endif /* ROMAN_H */