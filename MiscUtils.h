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
 * @brief Converts a UTF-8 encoded string to a wide-character string.
 *
 * This function converts a null-terminated UTF-8 string into a newly
 * allocated wide-character string (`wchar_t *`) using the Windows
 * `MultiByteToWideChar` API.
 *
 * @param utf8
 *  Pointer to a null-terminated UTF-8 encoded string.
 *
 * @return
 *  - Pointer to a newly allocated wide-character string on success.
 *  - NULL on failure, including:
 *      - Invalid UTF-8 sequences
 *      - Memory allocation failure
 *      - Conversion errors
 *
 * @note
 *  - The returned string is heap-allocated and must be freed by the caller.
 *  - If `utf8` is NULL, the function returns NULL immediately.
 */
wchar_t *utf8_to_wide(const char *utf8);


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
 * @brief Prints a wide-character string to the Windows console.
 * 
 * This function writes the given `wchar_t` string directly to the console using
 * the Windows API `WriteConsoleW()`. It avoids issues with the C runtime
 * and the current console code page, ensuring that Unicode characters (such as
 * Japanese kanji) are displayed correctly.
 * 
 * @param text Pointer to a null-terminated wide-character string to be printed.
 *             If NULL, the function does nothing.
 * 
 * @note The function writes a newline after the string.
 * @note If `GetStdHandle(STD_OUTPUT_HANDLE)` fails, it falls back to `wprintf()`.
 * 
 * @example
 * wchar_t *japanese = L"日本語";
 * print_wide(japanese);
 */
void print_wide(const wchar_t *text);


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
char *strip_leading_zeroes(const char *number);


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
 * truncates it at the first occurrence of a carriage return (`'\r'`).
 * It is primarily used to sanitize input read from Windows-style
 * text files (CRLF line endings) or copy-pasted content, where a
 * trailing `'\r'` can cause string comparison and parsing failures.
 *
 * The function modifies the input string in place.
 *
 * @param s Pointer to a null-terminated character buffer to sanitize.
 *
 * @note Only the first carriage return is removed. Characters after
 *       the `'\r'` are discarded.
 */
void strip_carriage_return(char *s);


#endif /* MISCUTILS_H */