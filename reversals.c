#include "reversals.h"

/**
 *      All things that have to do with reversals
 *      Palindrome - Number that looks the same after reversal
 *      Mirror Numbers - Number strings that have been reversed
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


// Use only verified numerical input for all functions

bool is_palindrome(const char *number)
{
    int n = strlen(number);
    if (number[n - 1] == '0') return false;

    // Strip leading zeroes
    int offset = -1;
    while (number[++offset] == '0');

    if (n - offset == 1)
        return false;   // Single-digit number

    for (
        char *left = number + offset, *right = number + n - 1; 
        left < right; 
        left++, right--
    ) {
        if (!isdigit(*left) || *left != *right)
        {
            return false;
        }
    }
    return true;
}


char *next_palindrome(const char *number)
{
    char *numberStripped, *nextNumber;

    // Strip leading zeroes from number
    numberStripped = number - 1;
    while (
        *++numberStripped == '0'
    );

    // Attempt malloc to new number string
    int n = strlen(numberStripped);
    nextNumber = malloc((n + 3) * sizeof(char));
    if (!nextNumber)
    {
        printf("Error - malloc failure");
        return NULL;
    }

    if (is_palindrome(numberStripped))
    {
        strcpy(nextNumber, numberStripped);
    }
    else
    {
        // Trivial case: single-digit number
        if (n < 2)
        {
            strcpy(nextNumber, "11");
            return nextNumber;
        }

        // Create palindrome from first (n+1)/2 characters
        memcpy(nextNumber, numberStripped, (n+1)/2);
        for (int i = n/2; i > 0; i--)
        {
            nextNumber[n - i] = numberStripped[i - 1];
        }
        nextNumber[n] = '\0';

        if (strcmp(nextNumber, numberStripped) > 0)
            return nextNumber;
    }

    for (
        int left = (n-1)/2, right = n/2;
        left >= 0 && right < n;
        left--, right++
    ) {
        if (nextNumber[left]++ == '9')
        {
            nextNumber[left] = '0';
            nextNumber[right] = '0';
            continue;
        }
        else if (left != right)
        {
            nextNumber[right]++;
        }
        break;
    }

    if (nextNumber[0] == '0')
    {
        nextNumber[0]++;
        nextNumber[n] = '1';
        nextNumber[n + 1] = '\0';
    }

    return nextNumber;
}


char *next_mirror_number(char *number, char mode)
{
    // Mode refers to mode of input
    // Two modes: mirror (m) and normal (n)
    // Mirror: inputted number is mirrored
    // Normal: inputted number is normal

    int n = strlen(number);
    char *nextNumber;

    switch(mode)
    {
        case 'm':
            // No stripping in mirrored mode due to ambiguity
            if (number[n - 1] == '0')
            {
                printf("Bad Input - Ambiguous trailing zeroes in mirrored number");
                return NULL;
            }

            nextNumber = malloc((n + 2) * sizeof(char));
            if (!nextNumber)
            {
                printf("Error - malloc failure");
                return NULL;
            }

            strcpy(nextNumber, number);

            for (int i = 0; i < n && nextNumber[i]++ == '9'; i++) 
            {
                nextNumber[i] = '0';  
            }

            if (nextNumber[n - 1] == '0') 
            {
                nextNumber[n] = '1';
                nextNumber[n + 1] = '\0';
            }
        break;

        case 'n':
            // Skip leading zeroes
            int offset = -1;
            while (
                number[++offset] == '0' && 
                offset < n - 1
            );

            nextNumber = malloc((n - offset + 2) * sizeof(char));
            if (!nextNumber)
            {
                printf("Error - malloc failure");
                return NULL;
            }

            strcpy(nextNumber, number + offset);

            for (int i = n - offset - 1; i >= 0 && nextNumber[i]++ == '9'; i--) 
            {
                nextNumber[i] = '0';
            }

            if (nextNumber[0] == '0')
            {
                nextNumber[n - offset] = '1';        // Convert directly into mirror
                nextNumber[n - offset + 1] = '\0';
                break;
            }

            // Finally, reverse order of string to return mirror
            for (
                char *left = nextNumber, *right = nextNumber + n - 1; 
                left < right; 
                left++, right--
            ) {
                char temp = *left;
                *left = *right;
                *right = temp;
            }
        break;
    }

    return nextNumber;
}