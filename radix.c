#include "radix.h"

/**
 *      Number Systems
 *      Utilities for conversions among number systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>


// Helper function: stringifies radix's name
const char *radix_name(enum Radix radix) 
{
    switch(radix)
    {
        case BINARY: return "binary";
        case DECIMAL: return "decimal";
        case HEXADECIMAL: return "hexadecimal";
        case OCTAL: return "octal";
        default: return "unknown-base";
    }
}


//  For inputted binary strings, ensure string length doesn't exceed 64 + 1
//  For inputted decimal strings, ensure string length doesn't exceed 20 + 1
//  For inputted hexadecimal strings, ensure string length doesn't exceed 16 + 1
//  For inputted octal strings, ensure string length doesn't exceed 22 + 1

//  BORDERLINE CASES
//  For inputted decimal strings of length 20 + 1, ensure string does not exceed 18446744073709551615
//  For inputted octal strings of length 22 + 1, ensure string does not exceed 1777777777777777777777


void number_to_string(char *buffer, unsigned long long number, enum Radix base)
{
    switch(base) 
    {
        case BINARY:
            int k = 0;

            // Construct string in reverse
            do
            {
                buffer[k++] = '0' + (number & 1);
                number >>= 1;
            }
            while (number != 0);
            
            buffer[k] = '\0';

            // Reverse final order to correct string
            for (
                int left = 0, right = k - 1; 
                left < right; 
                left++, right--
            ) {
                char temp = buffer[left];
                buffer[left] = buffer[right];
                buffer[right] = temp;
            }
        break;

        case DECIMAL:
            sprintf(buffer, "%llu", number);
        break;

        case HEXADECIMAL:
            sprintf(buffer, "%llX", number);
        break;

        case OCTAL:
            sprintf(buffer, "%llo", number);
        break;
    }
}


char *convert(const char *number, enum Radix convertFrom, enum Radix convertTo)
{
    unsigned long long value;
    char *numString, *endChar;
    errno = 0;

    if (number[0] == '-')
    {
        printf("Bad Input: Negative numbers not allowed");
        return NULL;
    }

    value = strtoull(number, &endChar, convertFrom);

    if (*endChar != '\0')
    {
        printf("Bad Input: Invalid %s number", radix_name(convertFrom));
        return NULL;
    }

    if (errno == ERANGE)
    {
        printf("Bad Input: Number too large");
        return NULL;
    }

    numString = malloc(65 * sizeof(char));
    if (!numString)
    {
        printf("Error - malloc failure");
        return NULL;
    }

    number_to_string(numString, value, convertTo);

    return numString;
}


char *next_number(const char *number, enum Radix base)
{
    // Ensure input is already sanitized
    char *nextNumber;
    unsigned long long value = strtoull(number, NULL, base);    // Error check not necessary since input is already sanitized
    
    if (value == ULLONG_MAX)  // Subtle detail: value increments here
    {
        printf("Bad Input - Maximum allowed number");
        return NULL;
    }

    nextNumber = malloc(65 * sizeof(char));
    if (!nextNumber)
    {
        printf("Error - malloc failure");
        return NULL;
    }

    number_to_string(nextNumber, value + 1, base);

    return nextNumber;
}