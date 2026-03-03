#include "ParamParser.h"

#include <stdio.h>
#include <string.h>

#include "ModeSelector.h"


char parse_radix_param(const char *arg, enum CountMode mode)
{
    if (strcmp(arg, "binary") == 0 || strcmp(arg, "bin") == 0 || strcmp(arg, "b") == 0)
        return (mode != MODE_BINARY) ? 'b' : '\0';

    if (strcmp(arg, "decimal") == 0 || strcmp(arg, "dec") == 0 || strcmp(arg, "d") == 0)
        return (mode != MODE_DECIMAL) ? 'd' : '\0';

    if (strcmp(arg, "hexadecimal") == 0 || strcmp(arg, "hex") == 0 || strcmp(arg, "h") == 0)
        return (mode != MODE_HEXADECIMAL) ? 'h' : '\0';

    if (strcmp(arg, "octal") == 0 || strcmp(arg, "oct") == 0 || strcmp(arg, "o") == 0)
        return (mode != MODE_OCTAL) ? 'o' : '\0';

    return '\0';    // None of the above
}

char parse_meme_param(const char *arg)
{
    if (strcmp(arg, "69") == 0)
        return (char)69;

    if (strcmp(arg, "420") == 0)
        return (char)420;

    if (strcmp(arg, "69420") == 0)
        return (char)69420;

    return '\0';    // None of the above
}

char parse_mirror_param(const char *arg)
{
    if (strcmp(arg, "m") == 0 || strcmp(arg, "mirror") == 0)
        return 'm';

    if (strcmp(arg, "n") == 0 || strcmp(arg, "normal") == 0)
        return 'n';

    return '\0';    // None of the above
}

char parse_roman_param(const char *arg)
{
    if (strcmp(arg, "l") == 0 || strcmp(arg, "legal") == 0)
        return 'l';

    if (strcmp(arg, "i") == 0 || strcmp(arg, "idiomatic") == 0)
        return 'i';

    return '\0';
}

char parse_tally_param(const char *arg)
{
    if (strcmp(arg, "b") == 0 || strcmp(arg, "big") == 0)
        return 'b';

    if (strcmp(arg, "c") == 0 || strcmp(arg, "compact") == 0)
        return 'c';

    if (strcmp(arg, "d") == 0 || strcmp(arg, "default") == 0)
        return 'd';

    if (strcmp(arg, "g") == 0 || strcmp(arg, "giant") == 0)
        return 'g';

    if (strcmp(arg, "t") == 0 || strcmp(arg, "tiny") == 0)
        return 't';

    return '\0';
}