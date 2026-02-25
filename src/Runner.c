#include "Runner.h"

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <wchar.h>
#include <windows.h>

#include "ClipboardFunctions.h"
#include "MiscUtils.h"


struct Func_Call *dispatcher(struct Func_Call call)
{
    void *res;
    char buffer[32];        // Optional buffer

    struct Func_Call *payload = calloc(1, sizeof *payload);
    if (!payload)
    {
        printf("Error - calloc failure.\n");
        return NULL;
    }

    switch (call.mode)
    {
        case EMOJI_MODE:
        case MORSE_MODE:
        {
            if (!call.arg.num_char_ptr)
            {
                printf("Error - Malformed call to runner dispatcher.\n");
                free(payload);
                return NULL;
            }

            size_t n = strlen(call.arg.num_char_ptr);

            /**
             *  Signatures that match:
             *      number_to_emoji()
             *      translate_to_morse_code()
             */
            char *(*form)(const char *) = call.func.formatter;

            payload->mode = call.mode;
            payload->func.formatter = form;
            
            payload->arg.num_char_ptr = malloc(n + 1);
            if (!payload->arg.num_char_ptr)
            {
                printf("Error - malloc failure.\n");
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
        case INCREASING_MODE:
        case PALINDROME_MODE:
        {
            if (!call.arg.num_char_ptr)
            {
                printf("Error - Malformed call to runner dispatcher.\n");
                free(payload);
                return NULL;
            }

            /**
             *  Signatures that match:
             *      next_69_number()
             *      next_420_number()
             *      next_69420_number()
             *      next_increasing_number()
             *      next_palindrome()
             */
            char *(*incr)(const char *) = call.func.incrementer;

            payload->mode = call.mode;
            payload->func.incrementer = incr;

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
                printf("Error - Malformed call to runner dispatcher.\n");
                free(payload);
                return NULL;
            }

            size_t n = strlen(call.arg.num_char_ptr);

            // Signature matches translate_to_japanese()
            wchar_t *(*form)(const char *) = call.func.formatter;

            payload->mode = call.mode;
            payload->func.formatter = form;

            payload->arg.num_char_ptr = malloc(n + 1);
            if (!payload->arg.num_char_ptr)
            {
                printf("Error - malloc failure.\n");
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
                printf("Error - Malformed call to runner dispatcher.\n");
                free(payload);
                return NULL;
            }

            // Signature matches next_mirror_number()
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

        case NOREP_MODE:
        {
            // Signature matches next_non_repeating_number()
            long long (*incr)(unsigned long long) = call.func.incrementer;

            payload->mode = call.mode;
            payload->func.incrementer = incr;

            payload->arg.num_llong = incr(call.arg.num_llong);
            if (payload->arg.num_llong == -1)
            {
                free(payload);
                return NULL;
            }

            sprintf(buffer, "%lld", payload->arg.num_llong);

            res = (void *)buffer;
            break;
        }

        case NWNS_MODE:
        case RADIX_MODE:
        {
            if (
                !call.arg.num_char_ptr || 
                !call.extra_args
            ) {
                printf("Error - Malformed call to runner dispatcher.\n");
                free(payload);
                return NULL;
            }

            /**
             *  Matching signatures:
             *      nwns()
             *      next_number()
             */
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
        
        case REP_MODE:
        {
            // Match signature to next_repeating_number()
            unsigned long long (*incr)(unsigned long long) = call.func.incrementer;

            payload->mode = call.mode;
            payload->func.incrementer = incr;

            payload->arg.num_ullong = incr(call.arg.num_ullong);
            if (!payload->arg.num_ullong)
            {
                free(payload);
                return NULL;
            }

            sprintf(buffer, "%llu", payload->arg.num_ullong);
        
            res = (void *)buffer;
            break;
        }

        case ROMAN_MODE:
        {
            if (
                !call.arg.num_char_ptr || 
                !call.extra_args
            ) {
                printf("Error - Malformed call to runner dispatcher.\n");
                free(payload);
                return NULL;
            }

            size_t n = strlen(call.arg.num_char_ptr);

            // Signature matches number_to_roman()
            wchar_t *(*form)(const char *, char) = call.func.formatter;

            payload->mode = call.mode;
            payload->func = call.func;

            payload->arg.num_char_ptr = malloc(n + 1);
            if (!payload->arg.num_char_ptr)
            {
                printf("Error - malloc failure.\n");
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

            payload->extra_args = call.extra_args;

            res = (void *)form(
                payload->arg.num_char_ptr, 
                call.extra_args[0]
            );
            break;
        }

        case TALLY_MODE:
        {
            if (
                !call.extra_args || 
                call.arg.num_llong == INT_MAX
            ) {
                printf("Error - Malformed call to runner dispatcher.\n");
                free(payload);
                return NULL;
            }

            // Signature matches tally()
            char *(*form)(int, int) = call.func.formatter;

            payload->mode = call.mode;
            payload->func = call.func;
            payload->arg.num_llong = call.arg.num_llong + 1;
            payload->extra_args = call.extra_args;

            res = (void *)form(
                payload->arg.num_llong, 
                payload->extra_args[0]
            );
            break;       
        }

        default:
            fprintf(stderr, "Unsupported return type\n");
            free(payload);
            return NULL;
    }

    if ((
        // Copying to clipboard failed
        (call.mode == JAPANESE_MODE || call.mode == ROMAN_MODE) ? 
            !copy_to_clipboard((wchar_t *)res) : 
            !copy_utf8_to_clipboard((char *)res)
        )
    ) {
        printf("Error - Could not copy value to clipboard.\n");
        free(payload);
        return NULL;
    }

    return payload;
}

    
/**
 *  Run Modes:
 *      r - run (normal duos run)
 *      s - solorun
 */
bool runner(struct Func_Call payload, char run_type)
{
    struct Func_Call *curr, *next;
    wchar_t *result;
    int iters = ('s' - run_type) + 1;
    bool ok = true;     // Return value

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD ir;
    DWORD eventsRead;

    // Optional: disable line buffering & echo
    DWORD mode;
    GetConsoleMode(hIn, &mode);
    SetConsoleMode(hIn, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

    // Run mode enum only needed for context
    curr = dispatcher(payload);
    if (!curr)
    {
        ok = false;
        goto cleanup;
    }

    result = read_clipboard();
    if (result)
    {
        print_wide(result);
        free(result);
    }

    printf(
        "Next value copied to clipboard!\n"
        "Press Tab to copy next value, "
        "or press Esc to end the run.\n"
    );

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
        {
            free(curr);
            goto cleanup;
        }

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
                for (int i = 0; i < iters; i++)
                {
                    next = dispatcher(*curr);
                    free(curr);
                    
                    if (!next)
                    {
                        ok = false;
                        goto cleanup;
                    }

                    curr = next;
                }

                printf("\n");

                result = read_clipboard();
                if (result)
                {
                    print_wide(result);
                    free(result);
                }

                printf(
                    "Next value copied to clipboard!\n"
                    "Press Tab to copy next value, "
                    "or press Esc to end the run.\n"
                );
            }
        }
    }

cleanup:
    SetConsoleMode(hIn, mode);
    return ok;
}