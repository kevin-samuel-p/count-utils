#ifndef OPTION_SELECTOR_H
#define OPTION_SELECTOR_H

#include "ModeSelector.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file OptionSelector.h
 * @brief Option parsing utilities for CLI command handling.
 *
 * Provides helper functions to validate and classify command-line
 * options associated with different counting modes.
 *
 * This module distinguishes between:
 *   - Run options (run / solorun)
 *   - Custom mode-specific options (convert / check / etc.)
**/

/**
 * @brief Determines whether the given option is a run-type option.
 *
 * Recognized run options:
 *   - "r"
 *   - "run"
 *   - "s"
 *   - "solorun"
 *
 * @param option Null-terminated string representing the CLI option.
 *
 * @return
 *   - 'r' if the option corresponds to run
 *   - 's' if the option corresponds to solorun
 *   - '\0' if the option is not a run-type option
 *
 * @note
 * The return value corresponds to the first character of the matched option.
 */
char is_run_option(const char *option);


/**
 * @brief Determines whether the given option is a valid custom option
 *        for the specified counting mode.
 *
 * Some modes support additional operations such as:
 *   - convert
 *   - check
 *   - emojify
 *   - translate
 *
 * The accepted options depend on the provided mode.
 *
 * @param option Null-terminated string representing the CLI option.
 * @param mode   The counting mode being evaluated.
 *
 * @return
 *   - 'c' if the option is a valid custom option for the given mode
 *   - '\0' if the option is invalid for the specified mode
 *
 * @note
 * The shorthand "c" is universally accepted for custom operations.
 *
 * @warning
 * The function does not validate NULL pointers. Passing NULL results
 * in undefined behavior.
 */
char is_custom_option(const char *option, enum CountMode mode);


#ifdef __cplusplus
}
#endif

#endif /* OPTION_SELECTOR_H */