#ifndef MISCUTILS_H
#define MISCUTILS_H

#include <wchar.h>

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


#endif /* MISCUTILS_H */