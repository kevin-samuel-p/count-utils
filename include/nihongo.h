#ifndef NIHONGO_H
#define NIHONGO_H

#include <wchar.h>

/**
 * @file nihongo.h
 * @brief Japanese numeral conversion utilities.
 *
 * Provides functions for converting between numerical strings
 * and Japanese numeral representations.
 *
 * Supported features:
 *  - Arabic numeral → Japanese numeral conversion
 *  - Japanese numeral → Arabic numeral conversion
 *
 * @note
 *      Numbers up to (but not including) 10^50 are supported.
**/


/**
 * @brief Japanese digit characters (0–9).
 *
 * Wide-character representations of digits used in Japanese numerals.
 *
 * @note
 *      This table is read-only and must not be modified.
 */
extern const wchar_t DIGITS[10];


/**
 * @brief Small unit characters used in Japanese numerals.
 *
 * Represents:
 *  - 十 (10)
 *  - 百 (100)
 *  - 千 (1000)
 *
 * @note
 *      Indexed in increasing order of magnitude.
 */
extern const wchar_t SMALL_UNITS[3];


/**
 * @brief Large unit characters used in Japanese numerals.
 *
 * Represents units from:
 *  - 万 (10^4)
 *  - 億 (10^8)
 *  - 兆 (10^12)
 *  - …
 *  - 極 (10^48)
 *
 * @note
 *      Indexed in increasing order of magnitude.
 */
extern const wchar_t LARGE_UNITS[12];


/**
 * @brief Converts a numerical string into Japanese numerals.
 *
 * @param number
 *      Null-terminated numerical string to convert.
 *
 * @pre
 *      - @p number contains digits only ('0'–'9')
 *      - No negative sign
 *      - Length ≤ 50 characters
 *
 * @return
 *      - Heap-allocated wide-character string on success
 *      - NULL on invalid input or allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
wchar_t *translate_to_japanese(const char *number);


/**
 * @brief Converts Japanese numerals into a numerical string.
 *
 * @param japaneseNumber
 *      Null-terminated wide-character Japanese numeral string.
 *
 * @pre
 *      - Input must follow valid Japanese numeric structure
 *
 * @return
 *      - Heap-allocated numerical string on success
 *      - NULL on invalid input or allocation failure
 *
 * @note
 *      This function does not modify the input buffer.
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *translate_from_japanese(const wchar_t *japaneseNumber);


#endif /* NIHONGO_H */