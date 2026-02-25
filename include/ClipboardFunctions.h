#ifndef CLIPBOARDFUNCTIONS_H
#define CLIPBOARDFUNCTIONS_H

#include <wchar.h>
#include <stdbool.h>

/**
 * @file ClipboardFunctions.h
 * @brief Utilities for copying text to the system clipboard (Windows only).
 *
 * This module provides functions for copying wide-character (UTF-16) and
 * UTF-8/ASCII strings to the Windows clipboard.
 *
 * @note
 *  - Windows-specific implementation; will not work on Linux/macOS.
 *  - Clipboard operations may fail if another application has locked the clipboard.
**/


/**
 * @brief Copies a wide-character string (UTF-16) to the system clipboard.
 *
 * @param text
 *      Null-terminated wide-character string to copy.
 *
 * @return
 *      - 1 if the copy was successful
 *      - 0 if the operation failed (clipboard could not be opened or memory allocation failed)
 */
bool copy_to_clipboard(const wchar_t *text);


/**
 * @brief Copies a UTF-8 or ASCII string to the system clipboard.
 *
 * This function internally converts the input string to wide-character (UTF-16)
 * and uses `copy_to_clipboard`.
 *
 * @param text
 *      Null-terminated UTF-8 or ASCII string to copy.
 *
 * @return
 *      - 1 if the copy was successful
 *      - 0 if the operation failed (conversion failure, memory allocation failure, or clipboard access error)
 */
bool copy_utf8_to_clipboard(const char *text);


/**
 * @brief Reads Unicode text from the Windows clipboard without modifying it.
 *
 * This function opens the system clipboard in read-only mode, retrieves
 * text stored in the CF_UNICODETEXT format, and returns a heap-allocated
 * copy of that text.
 *
 * The returned string is owned by the caller and must be released using
 * free(). The clipboard contents themselves are never altered.
 *
 * @return wchar_t*
 *         Pointer to a null-terminated wide-character string containing
 *         the clipboard text, or NULL if the clipboard does not contain
 *         Unicode text or an error occurs.
 *
 * @note The returned pointer remains valid after the clipboard is closed
 *       because the data is copied into caller-owned memory.
 *
 * @warning The caller is responsible for freeing the returned buffer.
 *
 * @see OpenClipboard
 * @see GetClipboardData
 * @see CF_UNICODETEXT
 */
wchar_t *read_clipboard(void);


#endif /* CLIPBOARDFUNCTIONS_H */