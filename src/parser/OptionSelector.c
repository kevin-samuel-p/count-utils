#include "OptionSelector.h"

#include <stdlib.h>
#include <string.h>

#include "ModeSelector.h"

char is_run_option(const char *option)
{
    if (
        strcmp(option, "r") == 0 ||
        strcmp(option, "s") == 0 ||
        strcmp(option, "run") == 0 ||
        strcmp(option, "solorun") == 0
    ) { return option[0]; }

    return '\0';    // Not a run option
}

char is_custom_option(const char *option, enum CountMode mode)
{
    if (strcmp(option, "c") == 0)
        return (
            mode != MODE_ALPHA &&
            mode != MODE_MEME &&
            mode != MODE_MIRROR &&
            mode != MODE_NWN &&
            mode != MODE_NWNWN &&
            mode != MODE_NWNWNN
        ) 
        ? 'c' : '\0';

    int ok = 0;

    switch(mode)
    {
        case MODE_EMOJI:
            if (
                strcmp(option, "e") == 0 || 
                strcmp(option, "emojify") == 0
            ) {
                ++ok;
                break;
            }
        
        case MODE_JAPANESE:
            if (
                strcmp(option, "t") == 0 ||
                strcmp(option, "translate") == 0
            ) {
                ++ok;
                break;
            }

        case MODE_BINARY:
        case MODE_DECIMAL:
        case MODE_HEXADECIMAL:
        case MODE_OCTAL:
        case MODE_TERNARY:

        case MODE_FACTORIZATION:
        case MODE_MORSE:
        case MODE_TALLY:

            if (strcmp(option, "convert") == 0)
                ++ok;
            break;
        
        case MODE_INCREASING:
        case MODE_PALINDROME:
        case MODE_REP:
        case MODE_NOREP:

            if (strcmp(option, "check") == 0)
                ++ok;
            break;
    }

    return (ok)
        ? 'c'
        : '\0'
    ;
}