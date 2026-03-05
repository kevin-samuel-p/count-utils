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
#include <errno.h>



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
        printf("Bad Input - Negative numbers not allowed.\n");
        return;
    }

    if (number[0] == '\0')
    {
        printf("Bad Input - Empty string.\n");
        return;
    }

    value = strtoull(number, &endChar, 10);

    if (*endChar != '\0')
    {
        printf("Bad Input - Invalid number.\n");
        return;
    }

    if (errno == ERANGE)
    {
        printf("Bad Input - Number too large.\n");
        return;
    }

    *v = malloc(sizeof(unsigned long long));
    if (!(*v))
    {
        printf("Error - malloc failure.\n");
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

void strip_leading_zeroes(char **number)
{
    // Only works for valid numbers
    if (!number || !*number)
    {
        printf("Bad Input - NULL ptr.\n");
        return;
    }

    char *num = *number;
    int n = strlen(num);

    int offset;
    for (
        offset = 0; 
        offset < n - 1 && *(num + offset) == '0'; 
        offset++
    );

    memmove(
        num, 
        num + offset, 
        n + 1 - offset
    );

    // Try realloc to save memory
    char *tmp = realloc(num, n + 1 - offset);
    if (tmp)
        *number = tmp;
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
        printf("Error - realloc failure.\n");
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

void strip_string(char **v)
{
    char *s = *v;
    const char DELIMITERS[] = " \t\n";

    // Remove newline characters and whitespace at the beginning and end of the string
    int n = strlen(s);
    int i;

    // lstrip
    i = strspn(s, DELIMITERS);
    memmove(s, s + i, n - i + 1);

    // Adjust new length
    n -= i;

    if (n == 0)
    {
        printf("Bad Input - No input was entered.\n");
        free(s);
        *v = NULL;
        return;
    }

    // rstrip
    for (
        i = n - 1;
        i >= 0 && strchr(DELIMITERS, s[i]);
        i--
    );
    s[i + 1] = '\0';

    // Attempt realloc
    char *temp = realloc(s, i + 2);
    if (temp)
        *v = temp;
}

bool sanitize(char **number, bool stripZeroes)
{
    strip_carriage_return(*number);
    strip_string(number);

    if (!number || !is_valid_number(*number)) 
        return false;

    if (stripZeroes)
        strip_leading_zeroes(number);

    return true;
}