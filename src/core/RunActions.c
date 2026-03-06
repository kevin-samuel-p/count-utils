#include "RunActions.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "platform.h"

#include "emoji.h"
#include "increasing.h"
#include "meme.h"
#include "morse.h"
#include "nihongo.h"
#include "nwnception.h"
#include "radix.h"
#include "reps.h"
#include "reversals.h"
#include "roman.h"
#include "tally.h"

#include "MiscUtils.h"
#include "ModeSelector.h"
#include "Runner.h"


bool run_radix(
    const char *starting_number, 
    char run_option,
    enum CountMode mode
) {
    enum Radix base;

    switch(mode)
    {
        case MODE_BINARY:
            base = BINARY;
        break;

        case MODE_DECIMAL:
            if (run_option == 'r')
            {
                printf("Sorry, decimal run mode is disabled at this time.\n");
                return false;
            }
            base = DECIMAL;
        break;

        case MODE_HEXADECIMAL:
            base = HEXADECIMAL;
        break;

        case MODE_OCTAL:
            base = OCTAL;
        break;
    }

    return runner(
        (struct Func_Call)
        {
            .mode = mode,
            .func.incrementer = next_number,
            .arg.num_char_ptr = starting_number,
            .extra_args = (char[]){(char)base, '\0'}
        }, 
        run_option
    );
}

bool run_emoji(const char *starting_number, char run_option)
{
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_EMOJI,
            .func.formatter = number_to_emoji,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );
}

bool run_increasing(const char *starting_number, char run_option)
{
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_INCREASING,
            .func.incrementer = next_increasing_number,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );
}

bool run_japanese(const char *string, char run_option, bool isNumber)
{
    char *starting_number;

    if (!isNumber)
    {
        wchar_t *input = utf8_to_wide(string);
        if (!input)
            return false;

        starting_number = translate_from_japanese(input);
        free(input);
    }
    else
    {
        starting_number = strdup(string);
    }

    if (!starting_number)
        return false;

    bool return_value = runner(
        (struct Func_Call)
        {
            .mode = MODE_JAPANESE,
            .func.formatter = translate_to_japanese,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );

    free(starting_number);
    return return_value;
}

bool run_meme(const char *starting_number, char run_option, char meme)
{
    void *incrementer;
    switch(meme)
    {
        case (char)(6+9):
            incrementer = next_69_number;
        break;

        case (char)(4+2+0):
            incrementer = next_420_number;
        break;

        case (char)(6+9+4+2+0):
            incrementer = next_69420_number;
        break;
    }

    return runner(
        (struct Func_Call)
        {
            .mode = MODE_MEME,
            .func.incrementer = incrementer,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );
}

bool run_mirror(
    const char *starting_number, 
    char run_option, 
    char starting_input_mode
) {
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_MIRROR,
            .func.incrementer = next_mirror_number,
            .arg.num_char_ptr = starting_number,
            .extra_args = (char[]){starting_input_mode, '\0'}
        },
        run_option
    );
}

bool run_morse(const char *string, char run_option, bool isNumber)
{
    char *starting_number;

    if (!isNumber)
        starting_number = translate_from_morse_code(string);
    else
        starting_number = strdup(string);
    
    if (!starting_number)
        return false;

    bool return_value = runner(
        (struct Func_Call)
        {
            .mode = MODE_MORSE,
            .func.formatter = translate_to_morse_code,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );

    free(starting_number);
    return return_value;
}

bool run_norep(
    unsigned long long starting_number, 
    char run_option
) {
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_NOREP,
            .func.incrementer = next_non_repeating_number,
            .arg.num_ullong = starting_number
        },
        run_option
    );
}

bool run_nwns(
    const char *starting_number, 
    char run_option, 
    enum CountMode mode
) {
    enum NWN_Mode nwn_mode;
    
    switch(mode)
    {
        case MODE_NWN:
            nwn_mode = NWN;
        break;

        case MODE_NWNWN:
            nwn_mode = NWNWN;
        break;

        case MODE_NWNWNN:
            nwn_mode = NWNWNN;
        break;
    }

    return runner(
        (struct Func_Call)
        {
            .mode = mode,
            .func.incrementer = nwns,
            .arg.num_char_ptr = starting_number,
            .extra_args = (char[]){(char)nwn_mode, '\0'}
        },
        run_option
    );
}

bool run_palindrome(const char *starting_number, char run_option)
{
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_PALINDROME,
            .func.incrementer = next_palindrome,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );
}

bool run_rep(unsigned long long starting_number, char run_option)
{
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_REP,
            .func.incrementer = next_repeating_number,
            .arg.num_ullong = starting_number
        },
        run_option
    );
}

bool run_roman(
    const char *string, 
    char run_option, 
    char representation,
    bool isNumber
) {
    char *starting_number;

    if (!isNumber)
    {
        wchar_t *input = utf8_to_wide(string);
        if (!input)
            return false;

        starting_number = roman_to_number(input);
        free(input);
    }
    else
    {
        starting_number = strdup(string);
    }

    if (!starting_number)
        return false;

    bool return_value = runner(
        (struct Func_Call)
        {
            .mode = MODE_ROMAN,
            .func.formatter = number_to_roman,
            .arg.num_char_ptr = starting_number,
            .extra_args = (char[]){representation, '\0'}
        },
        run_option
    );

    free(starting_number);
    return return_value;
}

bool run_tally(
    const char *string,
    char run_option,
    char formatting,
    bool isNumber
) {
    long long starting_number;
    unsigned long long *v = NULL;

    if (!isNumber)
    {
        starting_number = parse_tally_marks(string);
        if (starting_number == -1)
            return false;
    }
    else
    {
        string_to_number(string, &v);
        if (!v)
            return false;
        
        if (*v > INT_MAX)
        {
            printf("Bad Input - Number too large.\n");
            free(v);
            return false;
        }

        starting_number = (long long)(*v);
        free(v);
    }

    bool return_value = runner(
        (struct Func_Call)
        {
            .mode = MODE_TALLY,
            .func.formatter = tally,
            .arg.num_llong = starting_number,
            .extra_args = (char[]){formatting, '\0'}
        },
        run_option
    );

    return return_value;
}