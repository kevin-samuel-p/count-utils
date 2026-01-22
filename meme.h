#ifndef MEME_H
#define MEME_H

#include <stdbool.h>

/**
 * @file meme.h
 * @brief Utilities for working with “meme numbers”.
 *
 * Supports detection and generation of numbers containing
 * the digit sequences "69", "420", or either of the two.
 *
 * @note
 * All functions expect validated numerical input:
 *  - Null-terminated string
 *  - Digits only ('0'–'9')
 *  - No negative sign
**/


/**
 * @brief Checks whether a number contains the substring "69".
 *
 * @param number
 *      Null-terminated numerical string.
 *
 * @return
 *      - true  if the number contains "69"
 *      - false otherwise
 */
bool is_69_number(const char *number);


/**
 * @brief Checks whether a number contains the substring "420".
 *
 * @param number
 *      Null-terminated numerical string.
 *
 * @return
 *      - true  if the number contains "420"
 *      - false otherwise
 */
bool is_420_number(const char *number);


/**
 * @brief Generates the next number that contains the substring "69".
 *
 * The returned number is the smallest lexicographically greater
 * number than the input that contains "69".
 *
 * @param number
 *      Null-terminated numerical string.
 *
 * @return
 *      - Heap-allocated numerical string on success
 *      - NULL on allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *next_69_number(const char *number);


/**
 * @brief Generates the next number that contains the substring "420".
 *
 * The returned number is the smallest lexicographically greater
 * number than the input that contains "420".
 *
 * @param number
 *      Null-terminated numerical string.
 *
 * @return
 *      - Heap-allocated numerical string on success
 *      - NULL on allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *next_420_number(const char *number);


/**
 * @brief Generates the next number containing either "69" or "420".
 *
 * Returns whichever of the next "69" number or next "420" number
 * appears first in lexicographic order.
 *
 * @param number
 *      Null-terminated numerical string.
 *
 * @return
 *      - Heap-allocated numerical string on success
 *      - NULL on allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *next_69420_number(const char *number);


#endif /* MEME_H */