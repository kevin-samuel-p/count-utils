#include "MiscUtils.h"

/**
 *      Miscellaneous Utils
 *      Can be used for input parsing, type formatting, etc
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <errno.h>


wchar_t *utf8_to_wide(const char *utf8)
{
    if (!utf8)
        return NULL;

    int len = MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        utf8,
        -1,
        NULL, 
        0
    );

    if (len == 0)
        return NULL;

    wchar_t *wide = malloc((size_t)len * sizeof(wchar_t));
    if (!wide)
        return NULL;

    if (!MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8,
        -1,
        wide,
        len
    )) {
        printf("Bad Input - Input contains malformed UTF-8 sequences");
        free(wide);
        return NULL;
    }

    return wide;
}


void string_to_number(const char *number, unsigned long long **v)
{
    // This utility's scope is only upto positive decimal numbers <= ULLONG_MAX
    // No support for negative numbers
    // *v should be a NULL pointer
    // Memory will be alloced on the heap in case the number is processed completely (caller must free)

    if (*v) return;

    unsigned long long value;
    char *endChar;
    errno = 0;

    if (number[0] == '-')
    {
        printf("Bad Input: Negative numbers not allowed");
        return;
    }

    if (number[0] == '\0')
    {
        printf("Bad Input: Empty string");
        return;
    }

    value = strtoull(number, &endChar, 10);

    if (*endChar != '\0')
    {
        printf("Bad Input: Invalid number");
        return;
    }

    if (errno == ERANGE)
    {
        printf("Bad Input: Number too large");
        return;
    }

    *v = malloc(sizeof(unsigned long long));
    if (!(*v))
    {
        printf("Error: malloc failed");
        return;
    }

    **v = value;
}