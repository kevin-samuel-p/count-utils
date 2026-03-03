#include "RunActions.h"

#include <stdio.h>
#include <stdbool.h>

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

bool run_japanese(const char *starting_number, char run_option)
{
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_JAPANESE,
            .func.formatter = translate_to_japanese,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );
}

bool run_69(const char *starting_number, char run_option) 
{
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_MEME,
            .func.incrementer = next_69_number,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );
}

bool run_420(const char *starting_number, char run_option)
{
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_MEME,
            .func.incrementer = next_420_number,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );
}

bool run_69420(const char *starting_number, char run_option)
{
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_MEME,
            .func.incrementer = next_69420_number,
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

bool run_morse(const char *starting_number, char run_option)
{
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_MORSE,
            .func.formatter = translate_to_morse_code,
            .arg.num_char_ptr = starting_number
        },
        run_option
    );
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
    enum CountingMode mode
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
    const char *starting_number, 
    char run_option, 
    char representation
) {
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_ROMAN,
            .func.formatter = number_to_roman,
            .arg.num_char_ptr = starting_number,
            .extra_args = (char[]){representation, '\0'}
        },
        run_option
    );
}

bool run_tally(
    long long starting_number,
    char run_option,
    char formatting
) {
    return runner(
        (struct Func_Call)
        {
            .mode = MODE_TALLY,
            .func.formatter = tally,
            .arg.num_llong = starting_number,
            .extra_args = (char[]){formatting, '\0'}
        },
        run_option
    );
}