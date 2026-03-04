#ifndef MODE_SELECTOR_H
#define MODE_SELECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ModeSelector.h
 * @brief Mode selector definitions for counting modes.
 * 
 * This header defines the available counting modes and provides a function
 * to map string commands to their corresponding mode IDs.
**/

/**
 * @enum CountMode
 * @brief Enumeration of all supported counting modes.
 */
enum CountMode
{
    MODE_HELP = 1,     /**< Help mode */
    MODE_BINARY,       /**< Binary counting mode */
    MODE_DECIMAL,      /**< Decimal counting mode */
    MODE_EMOJI,        /**< Emoji counting mode */
    MODE_HEXADECIMAL,  /**< Hexadecimal counting mode */
    MODE_INCREASING,   /**< Increasing numbers counting mode */
    MODE_JAPANESE,     /**< Japanese numerals counting mode */
    MODE_MEME,         /**< Meme numbers (69, 420) counting mode */
    MODE_MIRROR,       /**< Mirror numbers counting mode */
    MODE_MORSE,        /**< Morse code counting mode */
    MODE_NOREP,        /**< Non-repeating numbers counting mode */
    MODE_NWN,          /**< Numbers-Within-Numbers counting mode */
    MODE_NWNWN,        /**< Numbers-Within-Numbers-Within-Numbers mode */
    MODE_NWNWNN,       /**< Numbers-Within-Numbers With N Numbers mode */
    MODE_OCTAL,        /**< Octal counting mode */
    MODE_PALINDROME,   /**< Palindrome counting mode */
    MODE_REP,          /**< Repeating numbers counting mode */
    MODE_ROMAN,        /**< Roman numerals counting mode */
    MODE_TALLY         /**< Tally marks counting mode */
};

/**
 * @brief Finds the numeric ID of a counting mode given its string name.
 * 
 * This function performs a binary search on the internal mode table
 * and returns the corresponding CountMode enum value.
 * 
 * @param cmd The null-terminated string representing the mode name.
 * @return The corresponding CountMode ID if found; -1 if the mode name is invalid.
 */
int find_mode(const char *cmd);

#ifdef __cplusplus
}
#endif

#endif /* MODE_SELECTOR_H */