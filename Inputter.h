#ifndef INPUTTER_H
#define INPUTTER_H

#include <stdio.h>

/**
 * @file Inputter.h
 * @brief Utilities for handling user input via temporary files.
 *
 * This module allows creating a temporary file, opening it for user input,
 * and deleting it afterward. It is designed for simple user interaction
 * where input is typed in a text editor.
 *
 * @note
 *  - The current implementation uses Notepad on Windows.
 *  - Temporary file is defined by `TEMP_FILE_NAME`.
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
 *  - 1 on success
 *  - 0 on failure (file could not be created)
 */
int create_temp_file(void);


/**
 * @brief Opens the temporary file in Notepad and waits for user input.
 *
 * The user can type the input and save the file. This function blocks
 * until Notepad is closed.
 *
 * @return
 *  - 1 if Notepad exited successfully
 *  - 0 if there was an error launching Notepad
 */
int await_user_input(void);


/**
 * @brief Deletes the temporary input file.
 *
 * @return
 *  - 1 on successful deletion
 *  - 0 if the file could not be deleted
 */
int delete_temp_file(void);


#endif /* INPUTTER_H */