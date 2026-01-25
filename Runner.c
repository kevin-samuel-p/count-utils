#include "Runner.h"

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>
#include <windows.h>

#include <ClipboardFunctions.h>
#include <MiscUtils.h>


// #define DISPATCH_RETURN(ret_type, value)                            \
//     do {                                                            \
//         switch (ret_type) {                                         \
//             case RET_INT:                                           \
//                 return (void *)(intptr_t)(value);                   \
//             case RET_BOOL:                                          \
//                 return (void *)(intptr_t)(value);                   \
//             case RET_CHAR_PTR:                                      \
//                 return (void *)(value);                             \
//             case RET_WCHAR_PTR:                                     \
//                 return (void *)(value);                             \
//             case RET_VOID:                                          \
//                 return NULL;                                        \
//             default:                                                \
//                 return NULL;                                        \
//         }                                                           \
//     } while (0)


struct Func_Call *dispatcher(struct Func_Call call)
{
    void *res;

    struct Func_Call *payload = calloc(1, sizeof *payload);
    if (!payload)
        return NULL;

    switch (call.mode)
    {
        case EMOJI_MODE:
        case MORSE_MODE:
        {
            if (!call.arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            size_t n = strlen(call.arg.num_char_ptr);

            // Match signature to number_to_emoji()
            char *(*form)(const char *) = call.func.formatter;

            payload->mode = call.mode;
            payload->func.formatter = form;
            
            payload->arg.num_char_ptr = malloc(n + 1);
            if (!payload->arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            memcpy(payload->arg.num_char_ptr, call.arg.num_char_ptr, n + 1);

            increment_numstring(&payload->arg.num_char_ptr);
            if (!payload->arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            res = (void *)form(payload->arg.num_char_ptr); 
            break;
        }

        case MEME_MODE:
        {
            if (
                !call.extra_args || ( 
                    strcmp(call.extra_args, "69") != 0 && 
                    strcmp(call.extra_args, "420") != 0 && 
                    strcmp(call.extra_args, "69420") != 0
                )
            ) {
                free(payload);
                return NULL;
            }
        }
        case INCREASING_MODE:
        {
            if (!call.arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            // Match signature to next_increasing_number()
            char *(*incr)(const char *) = call.func.incrementer;

            payload->mode = call.mode;
            payload->func.incrementer = incr;
            if (call.mode != INCREASING_MODE)
                payload->extra_args = call.extra_args;  // For memes

            payload->arg.num_char_ptr = incr(call.arg.num_char_ptr);
            if (!payload->arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            res = (void *)payload->arg.num_char_ptr;
            break;
        }

        case JAPANESE_MODE:
        {
            if (!call.arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            size_t n = strlen(call.arg.num_char_ptr);

            // Match signature to translate_to_japanese()
            wchar_t *(*form)(const char *) = call.func.formatter;

            payload->mode = call.mode;
            payload->func.formatter = form;

            payload->arg.num_char_ptr = malloc(n + 1);
            if (!payload->arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            memcpy(payload->arg.num_char_ptr, call.arg.num_char_ptr, n + 1);

            increment_numstring(&payload->arg.num_char_ptr);
            if (!payload->arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            res = (void *)form(payload->arg.num_char_ptr);
            break;
        }

        case MIRROR_MODE:
        {
            if (
                !call.arg.num_char_ptr || 
                !call.extra_args || (
                    call.extra_args[0] != 'm' && 
                    call.extra_args[0] != 'n'
                )
            ) {
                free(payload);
                return NULL;
            }

            // Match signature to next_mirror_number()
            char *(*incr)(char *, char) = call.func.incrementer;

            // Instead of the usual method, we can store mirror numbers
            // More efficient for this particular mode
            payload->mode = call.mode;
            payload->func.incrementer = incr;
            payload->extra_args = "m";

            payload->arg.num_char_ptr = incr(
                call.arg.num_char_ptr, 
                call.extra_args[0]
            );

            if (!payload->arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            res = (void *)payload->arg.num_char_ptr;
            break;
        }

        case RADIX_MODE:
        {
            if (
                !call.arg.num_char_ptr || 
                !call.extra_args || (
                    call.extra_args[0] != (char)2 && 
                    call.extra_args[0] != (char)8 && 
                    call.extra_args[0] != (char)10 && 
                    call.extra_args[0] != (char)16
                )
            ) {
                free(payload);
                return NULL;
            }

            // Match signature to next_number()
            char *(*incr)(const char *, int) = call.func.incrementer;

            payload->mode = call.mode;
            payload->func.incrementer = incr;
            payload->extra_args = call.extra_args;  // extra_args will be passed as a const pointer so this is fine

            payload->arg.num_char_ptr = incr(
                call.arg.num_char_ptr, 
                (int)call.extra_args[0]
            );

            if (!payload->arg.num_char_ptr)
            {
                free(payload);
                return NULL;
            }

            res = (void *)payload->arg.num_char_ptr;
            break;
        }
        
        default:
            fprintf(stderr, "Unsupported return type\n");
            free(payload);
            return NULL;
    }
}

    
/**
 *  Run Modes:
 *      r - run (normal duos run)
 *      s - solorun
 */
bool runner(struct Func_Call payload, char run_type)
{
    void *temp, *next;
    
    // Run mode enum only needed for context
    next = dispatcher(&payload);
    if (!next)
        return false;

    // Return early if number doesn't get copied to clipboard
    if (
        !((run_mode == JAPANESE_MODE || run_mode == ROMAN_MODE) ? 
            copy_to_clipboard((wchar_t *)next) : 
            copy_utf8_to_clipboard((char *)next)
        )
    ) {
        printf("Error - Problem copying value, run cancelled...\n");
        free(next);
        return false;
    }

    printf(
        "\nNext value copied to clipboard!\n"
        "Press Tab to copy next value, "
        "or press Esc to end the run.\n"
    );

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD ir;
    DWORD eventsRead;

    // Optional: disable line buffering & echo
    DWORD mode;
    GetConsoleMode(hIn, &mode);
    SetConsoleMode(hIn, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

    while (true) 
    {
        ReadConsoleInput(hIn, &ir, 1, &eventsRead);

        if (ir.EventType != KEY_EVENT)
            continue;

        KEY_EVENT_RECORD key = ir.Event.KeyEvent;

        // Only care about key-down events
        if (!key.bKeyDown)
            continue;

        // Abort on Esc
        if (key.wVirtualKeyCode == VK_ESCAPE) 
            break;

        // Detect standalone Tab
        if (key.wVirtualKeyCode == VK_TAB) 
        {
            DWORD mods = key.dwControlKeyState;

            if (((mods & (
                    LEFT_ALT_PRESSED | 
                    RIGHT_ALT_PRESSED |
                    LEFT_CTRL_PRESSED | 
                    RIGHT_CTRL_PRESSED |
                    SHIFT_PRESSED)
                ) == 0)
            ) {
                for (int i = 0; i < ('s' - run_type) + 1; i++)
                {
                    // Alter payload depending on run mode
                    switch(run_mode)
                    {
                        case BINARY_MODE:

                        case EMOJI_MODE:
                        case INCREASING_MODE:
                        
                        break;
                        
                        case JAPANESE_MODE:

                        case MEME_MODE:
                        case MIRROR_MODE:
                        case MORSE_MODE:
                        case NOREP_MODE:
                        case NWN_MODE:
                        case NWNWN_MODE:
                        case NWNWNN_MODE:
                        case PALINDROME_MODE:
                        case REP_MODE:
                        case ROMAN_MODE:
                        case TALLY_MODE:

                    }
                }
            }
        }
    }

    return true;
}