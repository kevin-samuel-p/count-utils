#ifndef TALLY_H
#define TALLY_H

/**
 * @file tally.h
 * @brief Utilities for generating and parsing Discord-formatted tally marks.
 *
 * This module provides:
 *  - A formatter that converts integers into Discord-friendly tally mark text.
 *  - A parser that converts such text back into an integer.
 *
 * The format is designed to avoid Discord message length limits while remaining
 * human-readable.
**/


/**
 * @enum Formatting
 * @brief Controls how tally mark output is visually formatted in Discord.
 */
enum Formatting
{
    BIG,        /**< Uses Discord "##" formatter on each non-header line */
    COMPACT,    /**< No newline characters; space-separated output */
    GIANT,      /**< Uses Discord "#" formatter on each non-header line */
    IMPACT,     /**< Uses Discord "###" formatter on each non-header line */
    TINY_TEXT,  /**< Uses Discord "-#" formatter on each non-header line */
    DEFAULT     /**< Standard formatting with newlines */
};


/**
 * @brief String representing five tally marks.
 *
 * This corresponds to a strikethrough block containing four pipes, which is
 * visually interpreted as five tally marks.
 */
extern const char *FIVE_BAR;


/**
 * @brief String representing fifty tally marks.
 *
 * This is composed of ten FIVE_BAR blocks and is used to efficiently render
 * large tallies while minimizing message length.
 */
extern const char *FIFTY_LINE;


/**
 * @brief Parse a Discord-formatted tally mark message into an integer.
 *
 * The input string may optionally begin with a header of the form:
 *
 * @code
 * x(.5)K +
 * @endcode
 *
 * followed by one or more lines of tally marks.
 *
 * Formatting tokens such as "#", "##" and "-#" are ignored during parsing.
 *
 * @param textWall
 * A mutable, null-terminated string containing the tally message.
 * This function modifies the buffer during parsing.
 *
 * @return
 * The integer value represented by the tally marks, or -1 if the input
 * is malformed.
 *
 * @note
 * The caller retains ownership of @p textWall.
 */
int parse_tally_marks(char *textWall);


/**
 * @brief Generate a Discord-formatted tally mark message.
 *
 * For values >= 1000, a header is emitted using "K" or ".5K" notation.
 * For values < 1000, no header is included.
 *
 * The returned string is guaranteed (by design constraints) to fit within
 * Discord's default 2000-character message limit.
 *
 * @param number
 * A positive integer to be represented using tally marks.
 *
 * @param formatting
 * Controls the visual formatting applied to each tally line.
 *
 * @return
 * A heap-allocated, null-terminated string containing the formatted tally
 * message, or NULL on error.
 *
 * @warning
 * The caller is responsible for freeing the returned string using free().
 */
char *tally(int number, enum Formatting formatting);


#endif /* TALLY_H */