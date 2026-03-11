#ifndef RADIX_H
#define RADIX_H

/**
 * @file radix.h
 * @brief Number system (radix) utilities.
 *
 * Provides functions for:
 *  - Converting numeric strings between common bases
 *  - Iterating numbers within a given radix
**/


/**
 * @enum Radix
 * @brief Supported number bases.
 */
enum Radix
{
    /** Base-2 (binary) */
    BINARY = 2,

    /** Base-3 (ternary) */
    TERNARY = 3,
    
    /** Base-8 (octal) */
    OCTAL = 8,

    /** Base-10 (decimal) */
    DECIMAL = 10,

    /** Base-16 (hexadecimal) */
    HEXADECIMAL = 16
};


/**
 * @brief Converts an unsigned integer to a null-terminated string in the given radix.
 *
 * This function writes the textual representation of an unsigned long long
 * integer into the provided buffer using the specified numeric base.
 *
 * Supported bases include binary, decimal, hexadecimal, and octal. The
 * resulting string is always null-terminated.
 *
 * @note
 *  - The caller is responsible for providing a buffer large enough to hold
 *    the resulting string, including the null terminator.
 *  - No bounds checking is performed on the buffer.
 *  - Hexadecimal output uses uppercase letters (A–F).
 *
 * @param buffer
 *  Pointer to a writable character buffer where the result will be stored.
 *
 * @param number
 *  Unsigned integer value to convert.
 *
 * @param base
 *  Radix used for conversion.
 *  Supported values are:
 *  - BINARY
 *  - DECIMAL
 *  - HEXADECIMAL
 *  - OCTAL
 */
void number_to_string(
    char *buffer, 
    unsigned long long number, 
    enum Radix base
);


/**
 * @brief Converts a numeric string from one radix to another.
 *
 * @param number
 *      Null-terminated numeric string to convert.
 *
 * @param convertFrom
 *      Radix of the input string.
 *
 * @param convertTo
 *      Target radix for the output string.
 *
 * @pre
 *      - @p number must be null-terminated
 *      - Digits must be valid for @p convertFrom
 *      - Negative numbers are not allowed
 *
 * @note
 *      Self-conversion (@p convertFrom == @p convertTo) may be used
 *      for input sanitization.
 *
 * @return
 *      - Heap-allocated string in the target radix on success
 *      - NULL on invalid input or allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *convert(
    const char *number, 
    enum Radix convertFrom, 
    enum Radix convertTo
);


/**
 * @brief Returns the next number in sequence for a given radix.
 *
 * @param number
 *      Sanitized numeric string.
 *
 * @param base
 *      Radix of the input number.
 *
 * @pre
 *      - Input must already be sanitized for the given radix
 *      - Input value must be less than @c ULLONG_MAX
 *
 * @return
 *      - Heap-allocated string representing the next number
 *      - NULL on overflow or allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *next_number(const char *number, enum Radix base);


#endif /* RADIX_H */