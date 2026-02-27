#ifndef MISCUTILS_H
#define MISCUTILS_H

#include <wchar.h>
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
 * @brief Checks whether a string represents a valid non-negative decimal number.
 *
 * This function verifies that the input string consists exclusively of
 * ASCII decimal digit characters (`0`–`9`). It does not allow leading signs,
 * whitespace, decimal points, or any other non-digit characters.
 *
 * @param number
 *  Pointer to a null-terminated string to validate.
 *
 * @return
 *  - true  if the string is non-NULL and contains only decimal digits
 *  - false if the string is NULL or contains any non-digit characters
 *
 * @note
 *  - An empty string ("") is considered valid by this function.
 *  - This function does not perform numeric range checking.
 *  - Locale-independent: digit checks are restricted to ASCII digits.
 *
 * @see isdigit
 */
bool is_valid_number(const char *number);


/**
 * @brief Removes leading zeroes from a decimal number string.
 *
 * This function allocates and returns a new string containing the input
 * number with all leading `'0'` characters removed. At least one digit
 * is preserved, so an input consisting entirely of zeroes will result
 * in a single `"0"` character.
 *
 * The input string is assumed to represent a valid non-negative decimal
 * number (digits only). Any violation of this assumption results in
 * an error and a NULL return value.
 *
 * @param number
 *  Pointer to a null-terminated string containing a decimal number.
 *
 * @return
 *  - Pointer to a newly allocated null-terminated string with leading
 *    zeroes removed
 *  - NULL if memory allocation fails or if invalid characters are detected
 *
 * @note
 *  - The caller is responsible for freeing the returned string.
 *  - The input string must not be NULL.
 *  - This function does not accept signed numbers or non-decimal formats.
 *  - Validation occurs during copying, not during offset calculation.
 *
 * @warning
 *  - This function assumes the input has already been validated.
 */
void strip_leading_zeroes(char **number);


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
 * @brief Removes an embedded carriage return character from a string.
 *
 * This helper function scans the given null-terminated string and
 * replaces carriage returns (`'\r'`) with newline characters (`'\n'`).
 * It is primarily used to sanitize input read from Windows-style
 * text files (CRLF line endings) or copy-pasted content, where a
 * trailing `'\r'` can cause string comparison and parsing failures.
 *
 * The function modifies the input string in place.
 *
 * @param s Pointer to a null-terminated character buffer to sanitize.
 *
 * @note All `'\r'` characters are replaced with `'\n'` characters.
 */
void strip_carriage_return(char *s);


/**
 * @brief Removes padding (whitespaces and newlines) from the left and right of a string.
 * 
 * This helper functions scans the given null-terminated string 
 * from both the start and the end for a contiguous block of padding
 * characters (`" \\n"`) and removes this padding. It is primarily used
 * to sanitize input read from text files, where a trailing/leading 
 * whitespace or newline inadvertently inserted by the user might lead
 * to a true negative during parsing.
 * 
 * The function modifies the input string pointer's value, and might
 * attempt to reallocate the memory block.
 * 
 * @param v 
 *      Pointer to a pointer to a null-terminated character buffer
 *      to sanitize.
 */
void strip_string(char **v);

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
 * - Removes leading zeroes (may adjust the pointer).
 *
 * The pointer itself may be updated (e.g., advanced past stripped
 * characters), so a pointer to the string pointer must be provided.
 *
 * @param[in,out] number
 *     Address of a dynamically allocated, null-terminated string pointer.
 *     On success, the pointer may be modified to reference a normalized
 *     substring of the original buffer.
 *
 * @return true  if the resulting string is a valid number after sanitization.
 * @return false if the string is not a valid number.
 *
 * @note The input string must be mutable. The function does not allocate
 *       or free memory.
 */
bool sanitize(char **number);

#endif /* MISCUTILS_H */