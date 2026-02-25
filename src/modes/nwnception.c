#include "nwnception.h"

/**
 *      Numbers Within Numbers (NWN)
 *      Numbers Within Numbers Within Numbers (NWNWN)
 *      Numbers within Numbers with N Numbers (NWNWNN)
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>


void increment_nwn(long **arr, long *arrSize, enum NWN_Mode mode)
{
    long depth = *arrSize - 1;
    for (; depth > 0; depth--)
    {
        if (++(*arr)[depth] > (*arr)[depth - 1])
        {
            (*arr)[depth] = 1;
            continue;
        }
        break;
    }

    if (depth == 0)
    {
        if (mode == NWNWNN)
        {
            long *temp = realloc(*arr, (*arrSize + 1) * sizeof(long));
            if (!(*temp))
            {
                printf("Error - realloc failure.\n");
                free(*arr);
                return;
            }

            *arr = temp;
            (*arr)[0] = *arrSize + 1;
            (*arr)[(*arrSize)++] = 1L;
        }
        else
        {
            ++(*arr)[0];
        }
    }
}


char *stringify_nwn(const long *arr, long arrSize)
{
    char *numString = malloc(15 * (size_t)arrSize * sizeof(char));
    if (!numString)
    {
        printf("Error - malloc failure\n");
        return NULL;
    }
    numString[0] = '\0';

    char *number = malloc(15 * sizeof(char));
    if (!number)
    {
        printf("Error - malloc failure\n");
        free(numString);
        return NULL;
    }

    for (int i = 0; i < arrSize; i++)
    {
        if (i != 0)
            strcat(numString, ".");

        sprintf(number, "%ld", arr[i]);
        strcat(numString, number);
    }
    free(number);

    return numString;
}


char *nwns(char *numString, enum NWN_Mode mode)
{
    char *endPtr, *token, *nextNumString;
    long *arr, arrPtr = 0, size, x;
    errno = 0;

    token = strtok(numString, ".");
    x = strtol(token, &endPtr, 10);

    if (*endPtr != '\0')
    {
        printf("Bad Input - Non-numeral prtition in string.\n");
        return NULL;
    }
    
    if (x <= 0)
    {
        printf("Bad Input - Non-positive numerals not allowed.\n");
        return NULL;
    }
    
    if (errno == ERANGE || x == LONG_MAX)
    {
        printf("Bad Input - Numeral too large.\n");
        return NULL;
    }

    
    if (mode == NWNWNN)
    {
        size = x;
    }
    else
    {
        size = mode;
    }

    arr = malloc(size * sizeof(long));
    if (!arr)
    {
        printf("Error - malloc failure\n");
        return NULL;
    }

    arr[arrPtr++] = x;
    while ((token = strtok(NULL, ".")) != NULL && arrPtr < size)
    {
        errno = 0;
        arr[arrPtr] = strtol(token, &endPtr, 10);

        if (*endPtr != '\0')
        {
            printf("Bad Input - Non-numeral partition in string.\n");
            free(arr);
            return NULL;
        }
        
        if (arr[arrPtr] <= 0)
        {
            printf("Bad Input - Non-positive numerals not allowed.\n");
            free(arr);
            return NULL;
        }
        
        if (errno == ERANGE || arr[arrPtr] == LONG_MAX)
        {
            printf("Bad Input - Numeral too large.\n");
            free(arr);
            return NULL;
        }
        
        if (arr[arrPtr] > arr[arrPtr - 1])
        {
            printf("Bad Input - Numeral partitions are not in non-increasing order.\n");
            free(arr);
            return NULL;
        }

        ++arrPtr;
    }

    if (arrPtr != size)
    {
        printf("Bad Input - Too few partitions for %ld numerals.\n", size);
        free(arr);
        return NULL;
    }

    if (token != NULL)
    {
        printf("Bad Input - Too many partitions for %ld numerals.\n", size);
        free(arr);
        return NULL;
    }

    increment_nwn(&arr, &size, mode);
    if (!arr)
        return NULL;

    nextNumString = stringify_nwn(arr, size);
    free(arr);

    return nextNumString;
}