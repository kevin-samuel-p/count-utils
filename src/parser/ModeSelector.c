#include "ModeSelector.h";

#include <stdlib.h>
#include <string.h>

typedef struct 
{
    const char *name;
    int id;
} 
Command;

Command mode_table[] = 
{
    {"binary", MODE_BINARY},
    {"decimal", MODE_DECIMAL},
    {"emoji", MODE_EMOJI},
    {"hexadecimal", MODE_HEXADECIMAL},
    {"increasing", MODE_INCREASING},
    {"japanese", MODE_JAPANESE},
    {"morse", MODE_MORSE},
    {"norep", MODE_NOREP},
    {"normal", MODE_DECIMAL},
    {"nwn", MODE_NWN},
    {"nwnwn", MODE_NWNWN},
    {"nwnwnn", MODE_NWNWNN},
    {"octal", MODE_OCTAL},
    {"palindrome", MODE_PALINDROME},
    {"rep", MODE_REP},
    {"roman", MODE_ROMAN},
    {"tally", MODE_TALLY}
};

/* Comparator for bsearch */
int compare_commands(const void *a, const void *b) 
{
    const char *key = (const char *)a;
    const Command *cmd = (const Command *)b;

    return strcmp(key, cmd->name);
}

int find_mode(const char *cmd)
{
    Command *result = bsearch
    (
        cmd,
        mode_table,
        sizeof(mode_table) / sizeof(mode_table[0]),
        sizeof(Command),
        compare_commands
    );

    return (result)
        ? result->id
        : -1
    ;
}