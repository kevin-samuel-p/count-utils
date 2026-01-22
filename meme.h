/**
 *      Support for Memes:
 * 
 *      ->  69 numbers: numbers containing the digits "69" lexicographically
 *      ->  420 numbers: numbers containing the digits "420" lexicographically
 * 
 *      Optional sequence: 69/420 numbers - sequence consisting of both 69 and 420 numbers
 * 
 *      SIX SEVEN IS FORBIDDEN
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// NOTE: Use only verified numerical input for the following functions.

// Validation functions
bool is_69_number(char *number) { return strstr(number, "69"); }
bool is_420_number(char *number) { return strstr(number, "420"); }


char *next_69_number(char *number)
{
    int n = strlen(number);
    char *nextNumber, *pre69, *peak, *post69;
    
    nextNumber = (char *)malloc((n + 2) * sizeof(char));
    if (!nextNumber)
    {
        printf("Error - malloc failure");
        return NULL;
    }

    // Trivial case: number less than 69
    if (
        n < 2 || 
        n == 2 && strcmp(number, "69") < 0
    ) {
        strcpy(nextNumber, "69");
        return nextNumber;
    }

    // Trivial case: other two digit numbers
    if (n == 2)
    {
        strcpy(nextNumber, "169");
        return nextNumber;
    }

    strcpy(nextNumber, number);

    peak = strstr(nextNumber, "69");

    // Portion of the number follows the "69"
    if (peak && *(peak + 2) != '\0')
    {
        for (post69 = nextNumber + n - 1; post69 >= peak + 2; post69--)
        {
            if (*post69 == '9')
            {
                *post69 = '0';
            }
            else
            {
                (*post69)++;
                break;
            }
        }

        if (post69 < peak + 2) 
        {
            memcpy(peak, "70", 2);
            strcpy(nextNumber + n - 2, "69");
        }

        return nextNumber;
    }

    // There is no "69" in the number
    if (!peak)
    {
        if (strcmp(nextNumber + n - 2, "69") < 0)
        {
            strcpy(nextNumber + n - 2, "69");
            return nextNumber;
        }

        strcpy(nextNumber + n - 2, "69");
    }

    // Dealing with portion preceding "69" in the number
    pre69 = nextNumber + n - 3;
    if (*pre69 == '6')
    {
        strcpy(pre69 + 1, "90");
        return nextNumber;
    }

    for (;; pre69--)
    {
        (*pre69)++;

        if (pre69 > nextNumber && strncmp(pre69 - 1, "69", 2) == 0)
        {
            while (pre69 < nextNumber + n) 
            {
                *++pre69 = '0';
            }
            break;
        }

        if (*pre69 > '9')
        {
            *pre69 = '0';
        }
        else break;

        // Quit while ahead to avoid segfault
        if (pre69 == nextNumber)
        {
            memmove(nextNumber + 1, nextNumber, n + 1);
            nextNumber[0] = '1';
            break;
        }
    }

    return nextNumber;
}


char *next_420_number(char *number)
{
    int n = strlen(number);
    char *nextNumber, *pre420, *peak, *post420;

    nextNumber = (char *)malloc((n + 2) * sizeof(char));
    if (!nextNumber)
    {
        printf("Error - malloc failure");
        return NULL;
    }

    // Trivial case: number less than 420
    if (
        n < 3 ||
        n == 3 && strcmp(number, "420") < 0
    ) {
        strcpy(nextNumber, "420");
        return nextNumber;
    }

    // Trivial case: other three-digit numbers
    if (n == 3)
    {
        strcpy(nextNumber, "1420");
        return nextNumber;
    }

    strcpy(nextNumber, number);

    peak = strstr(nextNumber, "420");

    // Portion of the number follows "420"
    if (peak && *(peak + 3) != '\0')
    {
        for (post420 = nextNumber + n - 1; post420 >= peak + 3; post420--)
        {
            if (*post420 == '9')
            {
                *post420 = '0';
            }
            else
            {
                (*post420)++;
                break;
            }
        }

        if (post420 < peak + 3)
        {
            *(peak + 2) = '1';
            strcpy(nextNumber + n - 3, "420");
        }

        return nextNumber;
    }

    // There is no "420" in the number
    if (!peak)
    {
        if (strcmp(nextNumber + n - 3, "420") < 0)
        {
            strcpy(nextNumber + n - 3, "420");
            return nextNumber;
        }

        strcpy(nextNumber + n - 3, "420");
    }

    // Dealing with portion preceding "420" in the number
    pre420 = nextNumber + n - 4;
    
    if (*pre420 == '4')
    {
        strcpy(pre420 + 1, "200");
        return nextNumber;
    }

    for (;; pre420--)
    {
        (*pre420)++;

        if (pre420 > nextNumber && strncmp(pre420 - 1, "42", 2) == 0)
        {
            while (pre420 < nextNumber + n)
            {
                *++pre420 = '0';
            }
            break;
        }

        if (*pre420 > '9')
        {
            *pre420 = '0';
        }
        else break;

        // Quit while ahead to avoid segfault
        if (pre420 == nextNumber)
        {
            memmove(nextNumber + 1, nextNumber, n + 1);
            nextNumber[0] = '1';
            break;
        }
    }

    return nextNumber;
}


char *next_69420_number(char *number)
{
    char *next69Number = next_69_number(number);
    char *next420Number = next_420_number(number);
    
    if (!next69Number || !next420Number)
    {
        if (next69Number) free(next69Number);
        if (next420Number) free(next420Number);
        return NULL;
    }

    int m = strlen(next69Number);
    int n = strlen(next420Number);

    if (m < n)
    {
        free(next420Number);
        return next69Number;
    }
    
    if (m > n)
    {
        free(next69Number);
        return next420Number;
    }

    int comparison = strcmp(next69Number, next420Number);
    
    free((comparison < 0) ? next420Number : next69Number);
    return (comparison < 0) ? next69Number : next420Number;
}