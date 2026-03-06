#include "Runner.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <wchar.h>

#include "platform.h";
#include "ModeSelector.h";
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
        case MODE_EMOJI:
        case MODE_MORSE:
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

        case MODE_MEME:
        case MODE_INCREASING:
        case MODE_PALINDROME:
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

        case MODE_JAPANESE:
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

        case MODE_MIRROR:
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

        case MODE_NOREP:
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

        case MODE_NWN:
        case MODE_NWNWN:
        case MODE_NWNWNN:
        case MODE_BINARY:
        case MODE_DECIMAL:
        case MODE_HEXADECIMAL:
        case MODE_OCTAL:
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
        
        case MODE_REP:
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

        case MODE_ROMAN:
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

        case MODE_TALLY:
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
        (call.mode == MODE_JAPANESE || call.mode == MODE_ROMAN) ? 
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

    enable_raw_mode();

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

    print_wide(
        L"Next value copied to clipboard!\n"
        L"Press Tab to copy next value, "
        L"or press Esc to end the run.\n"
    );

    while (true) 
    {
        int key = read_key();

        // Abort on Esc
        if (key == KEY_ESC) 
        {
            free(curr);
            goto cleanup;
        }

        // Detect standalone Tab
        if (key == KEY_TAB) 
        {
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

            print_wide(
                L"Next value copied to clipboard!\n"
                L"Press Tab to copy next value, "
                L"or press Esc to end the run.\n"
            );
        }
    }

cleanup:
    restore_mode();
    return ok;
}