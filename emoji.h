#ifndef EMOJI_H
#define EMOJI_H

/**
 * @file emoji.h
 * @brief Emoji-based numeric formatting utilities.
 *
 * Provides functionality for converting numerical strings
 * into their corresponding emoji representations.
**/


/**
 * @brief UTF-8 emoji representations of digits 0–9.
 *
 * Each entry corresponds to the emoji representation
 * of its index value.
 *
 * @note
 * This array is read-only and must not be modified.
 */
extern const char * const EMOJI_DIGITS[];


/**
 * @brief Converts a numerical string into an emoji string.
 *
 * @param number
 *      Null-terminated numerical string to convert.
 *
 * @pre
 *      - @p number contains digits only ('0'–'9')
 *      - No leading zeroes (except the number "0")
 *      - No negative sign
 *
 * @return
 *      - Heap-allocated emoji string on success
 *      - NULL on allocation failure or invalid input
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *number_to_emoji(const char *number);


#endif /* EMOJI_H */