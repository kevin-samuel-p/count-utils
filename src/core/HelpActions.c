#include "HelpActions.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "platform.h"
#include "ModeSelector.h"

struct DocMapper
{
    enum CountMode mode;
    const char *docpath;
}
map[] =
{
    { 0, "/docs/Info.txt" },
    { MODE_HELP, "/docs/Help.txt" },
    { MODE_BINARY, "/docs/Binary.txt" },
    { MODE_DECIMAL, "/docs/Decimal.txt" },
    { MODE_EMOJI, "/docs/Emoji.txt" },
    { MODE_HEXADECIMAL, "/docs/Hexadecimal.txt" },
    { MODE_INCREASING, "/docs/Increasing.txt" },
    { MODE_JAPANESE, "/docs/Japanese.txt" },
    { MODE_MEME, "/docs/Meme.txt" },
    { MODE_MIRROR, "/docs/Mirror.txt" },
    { MODE_MORSE, "/docs/Morse.txt" },
    { MODE_NOREP, "/docs/Norep.txt" },
    { MODE_NWN, "/docs/NWN.txt" },
    { MODE_NWNWN, "/docs/NWNWN.txt" },
    { MODE_NWNWNN, "/docs/NWNWNN.txt" },
    { MODE_OCTAL, "/docs/Octal.txt" },
    { MODE_PALINDROME, "/docs/Palindrome.txt" },
    { MODE_REP, "/docs/Rep.txt" },
    { MODE_ROMAN, "/docs/Roman.txt" },
    { MODE_TALLY, "/docs/Tally.txt" }
};

bool read_docs(enum CountMode mode)
{
    // Set up environment variable with installer to project root to access docs
    const char *root = getenv("COUNTER_ROOT");

    char buffer[256];
    char chunk[2048];

    int n = 0;

    const char filepath[FILENAME_MAX];
    snprintf(
        filepath, 
        sizeof(filepath), 
        "%s%s", 
        root, 
        map[mode].docpath
    );

    FILE *fp = fopen(filepath, "r");

    if (!fp)
    {
        printf(
            "Error - Could not open docs file.\n"
            "Either the file is missing, or read permissions are lacking.\n"
        );
        return false;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (buffer[0] == '=')
        {
            printf("%s", chunk);
            printf(
                "\nPress Tab to continue,"
                " or press Esc to exit.\n"
            );

            enable_raw_mode();

            int keypress;
            do { keypress = read_key(); } 
            while (
                keypress != KEY_TAB &&
                keypress != KEY_ESC
            );
            
            restore_mode();
            n = 0;

            if (keypress == KEY_ESC) break;
            if (keypress == KEY_TAB) continue;
        }

        n += snprintf(
            chunk + n, 
            sizeof(chunk) - n, 
            "%s", buffer
        );
    }

    fclose(fp);

    return true;
}