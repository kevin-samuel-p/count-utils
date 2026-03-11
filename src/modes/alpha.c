/**
 *  Alphabet Numbers
 */

#include "alpha.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char *next_alphabet_number(const char *string)
{
    if (!string)
    {
        printf("Bad Input - NULL string.\n");
        return NULL;
    }

    int n = strlen(string);
    if (n == 0)
    {
        printf("Bad Input - Empty string.\n");
        return NULL;
    }

    char *nextNumber = malloc(n + 2);
    if (!nextNumber)
    {
        printf("Error - malloc failure.\n");
        return NULL;
    }

    int i;

    // Copy to string
    for (i = 0; i < n; i++)
    {
        if (!isalpha((unsigned char)string[i]))
        {
            printf("Bad Input - Invalid alphabet number.\n");
            free(nextNumber);
            return NULL;
        }
        nextNumber[i] = toupper((unsigned char)string[i]);
    }
    nextNumber[n] = '\0';

    // Increment
    for (--i; i >= 0 && nextNumber[i]++ == 'Z'; i--)
        nextNumber[i] = 'A';

    if (i == -1)
    {
        nextNumber[n] = 'A';
        nextNumber[n + 1] = '\0';
    }

    return nextNumber;    
}