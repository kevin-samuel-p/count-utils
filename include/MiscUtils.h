#ifndef MISCUTILS_H
#define MISCUTILS_H

#include <stdbool.h>

/**
 * @file MiscUtils.h
 * @brief Miscellaneous utility functions for string conversion and parsing.
 *
 * This module provides small helper utilities used across the project,
 * including UTF-8 to wide-character conversion and safe parsing of
 * unsigned integer values from strings.
 *
 * @note
 *  - This module is Windows-specific and relies on Win32 APIs.
 *  - Memory allocation is performed internally where noted; the caller
 *    is responsible for freeing returned memory.
**/


/**
 * @brief Parses a positive decimal string into an unsigned long long.
 *
 * This function attempts to parse a null-terminated decimal string
 * representing a non-negative integer into an `unsigned long long`.
 * If parsing succeeds, memory is allocated on the heap to store the
 * resulting value.
 *
 * @param number
 *  Null-terminated string containing a decimal representation of a number.
 * 
 * @param v
 *  Address of a pointer that will receive the allocated value.
 *  - `*v` must be NULL on entry.
 *  - On success, `*v` points to a heap-allocated `unsigned long long`.
 *
 * @note
 *  - Only positive decimal numbers are supported.
 *  - Negative numbers, empty strings, invalid characters, and overflow
 *    conditions are rejected.
 *  - On failure, no memory is allocated and `*v` remains unchanged.
 *  - The caller is responsible for freeing the allocated memory on success.
 */
void string_to_number(const char *number, unsigned long long **v);


/**
 * @brief Increments a numerical string by one.
 *
 * @param number
 *      Pointer to a heap-allocated null-terminated numerical string.
 *
 * @pre
 *      - @p number points to a valid heap-allocated string
 *
 * @post
 *      - On success, *@p number contains the incremented value
 *      - On allocation failure, *@p number is freed and set to NULL
 *
 * @note
 *      This function may reallocate the string.
 */
void increment_numstring(char **number);


/**
 * @brief Sanitizes and normalizes a numeric string in place.
 *
 * This function modifies the provided string pointer to remove unwanted
 * formatting and normalize its contents. The following operations are
 * performed in order:
 *
 * - Removes any trailing carriage return characters.
 * - Strips surrounding whitespace or formatting (may adjust the pointer).
 * - Validates that the resulting string represents a valid number.
 * - Optionally removes leading zeroes (may adjust the pointer).
 *
 * The pointer itself may be updated (e.g., advanced past stripped
 * characters), so a pointer to the string pointer must be provided.
 *
 * @param[in,out] number
 *     Address of a dynamically allocated, null-terminated string pointer.
 *     On success, the pointer may be modified to reference a normalized
 *     substring of the original buffer.
 * @param stripZeroes
 *      Boolean value flagging whether to strip leading zeroes or not.
 *
 * @return true  if the resulting string is a valid number after sanitization.
 * @return false if the string is not a valid number.
 *
 * @note The input string must be mutable. The function does not allocate
 *       or free memory.
 */
bool sanitize(char **number, bool stripZeroes);

#endif /* MISCUTILS_H */