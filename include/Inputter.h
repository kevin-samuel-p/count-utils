#ifndef INPUTTER_H
#define INPUTTER_H

#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>

/**
 * @file Inputter.h
 * @brief Utilities for handling user input via temporary files.
 *
 * This module allows creating a temporary file, opening it for user input,
 * reading the contents back into memory, and deleting the file afterward.
 *
 * Input is read as UTF-8 encoded text. ASCII-only modules may consume the
 * input directly, while Unicode-aware modules may convert it to wide
 * characters using the provided helper.
 *
 * @note
 *  - UTF-16 encoded files are explicitly rejected.
**/


/**
 * @brief Name of the temporary input file.
 *
 * Users are expected to enter their input into this file.
 */
extern const char TEMP_FILE_NAME[];


/**
 * @brief Creates a temporary file for user input.
 *
 * @return
 *  - true on success
 *  - false on failure (file could not be created)
 */
bool create_temp_file(void);


/**
 * @brief Opens the temporary file in Notepad and waits for user input.
 *
 * The user can type the input and save the file. This function blocks
 * until Notepad is closed.
 *
 * @return
 *  - true if Notepad exited successfully
 *  - false if there was an error launching Notepad
 */
bool await_user_input(void);


/**
 * @brief Reads the temporary input file as a UTF-8 encoded byte string.
 *
 * The file is read in binary mode and returned as a null-terminated
 * `char *` buffer. The function explicitly checks for UTF-16 byte-order
 * marks (BOMs) and fails early if one is detected.
 *
 * @note
 *  - The returned buffer contains raw UTF-8 bytes.
 *  - ASCII-only input is always valid UTF-8.
 *  - The caller is responsible for freeing the returned buffer.
 *
 * @return
 *  - Pointer to a heap-allocated UTF-8 string on success
 *  - NULL on failure (I/O error, allocation failure, or UTF-16 input)
 */
char *read_temp_file_utf8(void);


/**
 * @brief Deletes the temporary input file.
 *
 * @return
 *  - true on successful deletion
 *  - false if the file could not be deleted
 */
bool delete_temp_file(void);


#endif /* INPUTTER_H */