/**
 *      *Increasing Digits
 *      Given a number with its digits in non-decreasing order, find the next such number
 *      Validate input before calling functions
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


bool has_increasing_digits(char *);     // Finds whether digits are in non-decreasing order
char *next_increasing_number(char *);   // Iterative lexicographic generator


bool has_increasing_digits(char *number)
{
    // Use only verified input
    char prev = '\0';
    for (int i = 0; number[i] != '\0'; i++)
    {
        if (prev > number[i])
            return false;

        prev = number[i];
    }
    return true;
}


char *next_increasing_number(char *number)
{
    int n = strlen(number);
    char *buffer = (char *)malloc((n + 2) * sizeof(char));
    if (!buffer)
    {
        printf("Error - malloc failure");
        return NULL;
    }
    
    // If input number's digits are not in non-decreasing order, return next number early
    char lastDigit = '\0';
    int depth = 0;
    
    for (; depth < n; depth++)
    {
        if (lastDigit > number[depth])
        {  
            // Trivial case
            for (; depth < n; depth++)
            {
                buffer[depth] = lastDigit;
            }
            buffer[depth] = '\0';

            return buffer;
        }

        buffer[depth] = lastDigit = number[depth];
    }
    buffer[depth] = '\0';

    depth = n - 1;

    while (depth >= 0 && depth < n)
    {
        if (lastDigit == '9')
        {
            lastDigit = buffer[--depth];
        }
        else
        {
            buffer[depth++] = lastDigit + 1;
        }
    }

    if (depth == -1)
    {
        for (++depth; depth < n + 1; depth++)
        {
            buffer[depth] = '1';
        }
        buffer[n + 1] = '\0';
    }

    return buffer;
}