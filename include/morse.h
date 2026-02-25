#ifndef MORSE_H
#define MORSE_H

/**
 * @file morse.h
 * @brief Morse code utilities for numeric strings.
 *
 * Implements conversion between numerical strings and
 * their Morse code representations.
 *
 * Encoding rules:
 *  - Each digit is represented by a cluster of five symbols
 *  - Digit clusters are separated by single spaces
**/


/**
 * @brief Morse code representations of digits 0–9.
 *
 * Each entry contains a null-terminated string of
 * five Morse symbols representing the corresponding digit.
 *
 * @note
 * This lookup table is read-only and must not be modified.
 */
extern const char * const MORSE_CODE_DIGITS[10];


/**
 * @brief Translates a numerical string into Morse code.
 *
 * @param number
 *      Null-terminated numerical string to convert.
 *
 * @pre
 *      - @p number contains digits only ('0'–'9')
 *      - No negative sign
 *
 * @note
 *      Leading zeroes in the input are removed.
 *
 * @return
 *      - Heap-allocated Morse code string on success
 *      - NULL on invalid input or allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *translate_to_morse_code(const char *number);


/**
 * @brief Translates Morse code into a numerical string.
 *
 * Converts a Morse code string consisting of digit clusters
 * separated by spaces back into a numerical string.
 *
 * @param morseNumber
 *      Null-terminated Morse code string.
 *
 * @pre
 *      - Each digit cluster consists of exactly five symbols
 *      - Digit clusters are separated by single spaces
 *
 * @warning
 *      This function modifies the input buffer.
 *
 * @return
 *      - Heap-allocated numerical string on success
 *      - NULL on invalid input or allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *translate_from_morse_code(char *morseNumber);


#endif /* MORSE_H */