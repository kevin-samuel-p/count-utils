#ifndef RUNNER_H
#define RUNNER_H

#include <stdbool.h>
#include <wchar.h>

#include "ModeSelector.h"

/**
 * @file Runner.h
 * @brief Function-dispatch and interactive runner API.
 *
 * This module defines the core data structures and public entry points used
 * to dispatch numeric transformations, formatting operations, and clipboard
 * output based on a runtime-selected mode.
 *
 * This is a developer-facing API. Callers are responsible for ensuring that
 * function pointers and arguments match the selected RunMode.
**/


/**
 * @struct Func_Call
 * @brief Describes a callable transformation operation.
 *
 * A Func_Call fully describes how a value should be incremented, formatted,
 * and dispatched. It contains:
 *
 *  - a RunMode selecting behavior
 *  - a function pointer matching that mode
 *  - an argument union holding the current value
 *  - optional extra arguments
 *
 * Instances of this structure are passed by value into dispatcher() and
 * runner(), and new heap-allocated instances are returned to represent
 * updated state.
 */
struct Func_Call
{
    /** Running mode enum used to select function signatures */
    enum CountMode mode;

    /**
     * @union func
     * @brief Function pointer storage.
     *
     * Depending on the mode, exactly one of these is valid and must match
     * the expected signature for that mode.
     */
    union
    {
        void *formatter;    /**< Formatting / conversion function */
        void *incrementer;  /**< Increment / next-value function */
    }
    func;

    /**
     * @union arg
     * @brief Primary argument storage.
     *
     * Only one member is valid at a time, depending on the mode.
     */
    union
    {
        unsigned long long num_ullong; /**< Unsigned integer argument */
        long long          num_llong;  /**< Signed integer argument */
        char              *num_char_ptr;   /**< Heap-allocated UTF-8 string */
        wchar_t           *num_wchar_ptr;  /**< Heap-allocated wide string */
    }
    arg;

    /**
     * @brief Optional extra arguments.
     *
     * Interpretation depends on the mode:
     *  - radix values
     *  - enum-like characters
     *  - formatting flags
     *
     * This pointer is treated as read-only by the API.
     */
    char *extra_args;
};


/**
 * @brief Dispatches a single transformation step and copies the result to the clipboard.
 *
 * This function:
 *  - selects behavior based on @p call.mode
 *  - invokes the appropriate function pointer
 *  - performs any required increment or formatting
 *  - copies the result to the system clipboard
 *
 * On success, a new heap-allocated Func_Call representing the updated state
 * is returned.
 *
 * @param call
 *      Function-call descriptor specifying mode, function pointers, and arguments.
 *
 * @return
 *      - Pointer to a newly allocated Func_Call on success
 *      - NULL on failure (invalid arguments, allocation failure, or clipboard error)
 *
 * @pre
 *      - @p call.mode is valid
 *      - Function pointers in @p call.func match the selected mode
 *      - Required arguments are non-NULL and well-formed
 *
 * @post
 *      - Result is copied to the clipboard on success
 *      - Returned Func_Call must be freed by the caller
 *
 * @warning
 *      Passing an incompatible function pointer for a given mode results in
 *      undefined behavior.
 */
struct Func_Call *dispatcher(struct Func_Call call);


/**
 * @brief Runs an interactive transformation loop driven by keyboard input.
 *
 * The runner:
 *  - initializes the Windows console for raw input
 *  - dispatches an initial operation
 *  - waits for user input:
 *      - Tab → advance to next value(s)
 *      - Esc → exit the run
 *
 * Each successful iteration copies the new value to the clipboard.
 * The console mode is always restored before returning.
 *
 * @param payload
 *      Initial Func_Call descriptor.
 *
 * @param run_type
 *      Run selector:
 *      - 'r' : normal run (single iteration per Tab)
 *      - 's' : solo run (multiple iterations per Tab)
 *
 * @return
 *      - true  if the run ended normally via user input
 *      - false if execution aborted due to an internal failure
 *
 * @note
 *      Windows-only. Uses the Win32 console input API.
 */
bool runner(struct Func_Call payload, char run_type);


#endif /* RUNNER_H */
