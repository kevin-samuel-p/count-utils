#include "MiscUtils.h"

/**
 *      Miscellaneous Utils
 *      Can be used for input parsing, type formatting, etc
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include <windows.h>
#include <errno.h>


wchar_t *utf8_to_wide(const char *utf8)
{
    if (!utf8)
    {
        printf("Bad Input - NULL string.\n");
        return NULL;
    }

    int len = MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        utf8,
        -1,
        NULL, 
        0
    );

    if (len == 0)
    {
        printf("Error - Could not process multibyte string.\n");
        return NULL;
    }

    wchar_t *wide = malloc((size_t)len * sizeof(wchar_t));
    if (!wide)
    {
        printf("Error - malloc failure.\n");
        return NULL;
    }

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


void print_wide(const wchar_t *text)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD written;
        WriteConsoleW(hOut, text, (DWORD)wcslen(text), &written, NULL);
        WriteConsoleW(hOut, L"\n", 1, &written, NULL);
    }
    else
    {
        wprintf(L"%ls\n", text); // fallback
    }
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
        printf("Bad Input: Negative numbers not allowed.\n");
        return;
    }

    if (number[0] == '\0')
    {
        printf("Bad Input: Empty string.\n");
        return;
    }

    value = strtoull(number, &endChar, 10);

    if (*endChar != '\0')
    {
        printf("Bad Input: Invalid number.\n");
        return;
    }

    if (errno == ERANGE)
    {
        printf("Bad Input: Number too large.\n");
        return;
    }

    *v = malloc(sizeof(unsigned long long));
    if (!(*v))
    {
        printf("Error: malloc failed.\n");
        return;
    }

    **v = value;
}


bool is_valid_number(const char *number)
{
    if (!number) return false;

    for (int i = 0; number[i] != '\0'; i++)
        if (!isdigit((unsigned char)number[i]))
            return false;

    return true;
}


char *strip_leading_zeroes(const char *number)
{
    // Only works for valid numbers
    if (!number)
    {
        printf("Bad Input - NULL ptr");
        return NULL;
    }

    int n = strlen(number);
    char *strippedNumber;

    int offset;
    for (
        offset = 0; 
        offset < n - 1 && *(number + offset) == '0'; 
        offset++
    );

    strippedNumber = malloc((n - offset + 1) * sizeof(char));
    if (!strippedNumber)
    {
        printf("Error - malloc failure.\n");
        return NULL;
    }

    for (int i = offset; i < n; i++)
    {
        if (!isdigit((unsigned char)number[i]))
        {
            printf("Bad Input - Invalid number.\n");
            free(strippedNumber);
            return NULL;
        }

        strippedNumber[i - offset] = number[i];
    }
    strippedNumber[n - offset] = '\0';
    
    return strippedNumber;
}


void increment_numstring(char **number)
{
    int n = strlen(*number);
    for (int i = n - 1; i >= 0; i--)
    {
        if (++(*number)[i] > '9')
            (*number)[i] = '0';
        else 
            return;
    }

    char *temp = realloc(*number, n + 2);
    if (!temp)
    {
        printf("Error - realloc failed");
        free(*number);
        return;
    }

    (*number)[0] = '1';
    (*number)[n] = '0';
    (*number)[n + 1] = '\0';
}


// Helper function to strip embedded carriage returns
void strip_carriage_return(char *s)
{
    char *p = s;
    while (*p)
    {
        if (*p == '\r')
            *p = '\n';

        p++;
    }
}