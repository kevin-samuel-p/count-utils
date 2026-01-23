#include "nihongo.h"

/**
 *      Japanese Numbers
 *      Converts Japanese number to numerical number
 *      Converts numerical number to Japanese number
 */


#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


const wchar_t DIGITS[10] = 
{
    L'零', L'一', L'二', L'三', L'四',      // 0, 1, 2, 3, 4
    L'五', L'六', L'七', L'八', L'九'       // 5, 6, 7, 8, 9
};

const wchar_t SMALL_UNITS[3] = 
{
    L'十', L'百', L'千'                     // 10, 100, 1000
};

const wchar_t LARGE_UNITS[12] = 
{
    L'万', L'億', L'兆', L'京',             // 10^4, 10^8, 10^12, 10^16
    L'垓', L'秭', L'穣', L'溝',             // 10^20, 10^24, 10^28, 10^32
    L'澗', L'正', L'載', L'極'              // 10^36, 10^40, 10^42, 10^48
};


wchar_t *translate_to_japanese(const char *number)
{
    int n = strlen(number);
    if (n > 50)
    {
        printf("Bad Input - Number too large");
        return NULL;
    }

    wchar_t *japaneseNumber = malloc((2*n + 1) * sizeof(wchar_t));
    if (!japaneseNumber)
    {
        printf("Error - malloc failure");
        return NULL;
    }
    int japaneseNumberLen = 0;

    if (strcmp(number, "0") == 0)
    {
        japaneseNumber[0] = DIGITS[0];
        japaneseNumber[1] = L'\0';

        return japaneseNumber;
    }

    wchar_t *chunk = malloc(12 * sizeof(wchar_t));
    if (!chunk)
    {
        printf("Error - malloc failure");
        free(japaneseNumber);
        return NULL;
    }
    int chunkPtr;

    // Construct number in reverse
    for (int i = 1; 4*i <= n; i++)
    {
        chunkPtr = 0;
        for (int j = 0; j < 4; j++)
        {
            switch(number[n - 4*i + j])
            {
                case '0':
                    continue;

                case '1':
                    if (j < 3) break;
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    chunk[chunkPtr++] = DIGITS[number[n - 4*i + j] - '0'];
                break;

                default:
                    printf("Bad Input - Invalid number");
                    free(chunk);
                    free(japaneseNumber);
                return NULL;
            }
            
            if (j < 3)
            {
                chunk[chunkPtr++] = SMALL_UNITS[2 - j];
            }
        }

        if (i >= 2 && chunkPtr != 0)
        {
            chunk[chunkPtr++] = LARGE_UNITS[i - 2];
        }

        chunk[chunkPtr] = L'\0';

        for (int j = chunkPtr - 1; j >= 0; j--)
        {
            japaneseNumber[japaneseNumberLen++] = chunk[j];
        }
    }

    // Deal with initial block of digits, if length not divisible by 4
    chunkPtr = 0;
    for (int j = n % 4; j > 0; j--)
    {
        if (!isdigit(number[n%4 - j]))
        {
            printf("Bad Input - Invalid number");
            free(chunk);
            free(japaneseNumber);
            return NULL;
        }

        if (number[n%4 - j] == '0') 
            continue;

        if (number[n%4 - j] != '1' || j == 1) 
        {
            chunk[chunkPtr++] = DIGITS[number[n%4 - j] - '0'];
        }

        if (j >= 2)
        {
            chunk[chunkPtr++] = SMALL_UNITS[j - 2];
        }
    }

    if (n%4 != 0 && n/4 >= 1)
    {
        chunk[chunkPtr++] = LARGE_UNITS[n/4 - 1];
    }

    chunk[chunkPtr] = L'\0';
    for (int j = chunkPtr - 1; j >= 0; j--)
    {
        japaneseNumber[japaneseNumberLen++] = chunk[j];
    }
    free(chunk);

    japaneseNumber[japaneseNumberLen] = L'\0';

    // Reverse string order
    for (int left = 0, right = japaneseNumberLen - 1; left < right; left++, right--)
    {
        wchar_t temp = japaneseNumber[left];
        japaneseNumber[left] = japaneseNumber[right];
        japaneseNumber[right] = temp;
    }

    return japaneseNumber;
}


char *translate_from_japanese(const wchar_t *japaneseNumber)
{
    int n = wcslen(japaneseNumber);
    char *number;

    if (
        wcscmp(japaneseNumber, L"零") == 0 ||
        wcscmp(japaneseNumber, L"〇") == 0
    ) {
        number = malloc(2 * sizeof(char));
        if (!number)
        {
            printf("Error - malloc failure");
            return NULL;
        }

        strcpy(number, "0");
        return number;
    }

    int lastBlock = 12,         // Tracks the last big unit's index in LARGE_UNITS, 12 by default
        value = 0,              // Tracks the last digit typed
        block[4] = {0},         // Keeps track of the values in each chunk
        blockPtr = -1;          // Pointer to ensure no duplicate place values or unordered structure
    bool op;                    // Flag to check whether an operation was done or not

    number = malloc(53 * sizeof(char));
    if (!number)
    {
        printf("Error - malloc failure");
        return NULL;
    }

    for (int i = 0; i < 52; i++)
    {
        number[i] = '0';
    }
    number[52] = '\0';

    for (int i = 0; i < n; i++)
    {
        op = false;

        // If digit, alter value variable
        // Include check for bad sequences
        for (int j = 1; !op && j < 10; j++)
        {
            if (japaneseNumber[i] == DIGITS[j])
            {
                // Value should have been reset to base value of 0
                // If it wasn't, then a larger value succeeds a unit value -- invalid sequence
                if (value != 0)
                {
                    printf("Bad Input - Invalid number");
                    free(number);
                    return NULL;
                }

                value = j;
                op = true;
            }
        }

        // If 10x multiplier/small unit, update block and blockPtr after multiplication with value
        // Include check for bad sequences
        for (int j = 0; !op && j < 3; j++)
        {
            if (japaneseNumber[i] == SMALL_UNITS[j])
            {
                // For bad sequences involving wrong order, check blockPtr position
                if (blockPtr >= 2 - j)
                {
                    printf("Bad Input - Invalid number");
                    free(number);
                    return NULL;
                }

                blockPtr = 2 - j;
                block[blockPtr] = (value != 0) ? value : 1;
                
                value = 0;      // Reset value to default(0)
                op = true;
            }
        }

        /**
         *  If 10k multiplier/large unit,
         *      ->  Set final unit of chunk if (value != 0), then reset value
         *      ->  Assign digits as required
         *      ->  Update lastBlock
         *  Include check for bad sequences
         */
        for (int j = 0; !op && j < lastBlock; j++)
        {
            if (japaneseNumber[i] == LARGE_UNITS[j])
            {
                if (value != 0)
                {
                    blockPtr = 3;
                    block[blockPtr] = value;

                    value = 0;  // Reset value to default(0)
                }

                lastBlock = j;  // Update lastBlock value

                for (int k = 0; k < 4; k++)
                {
                    number[52 - 4*(j+2) + k] = '0' + block[k];
                }

                memset(block, 0, sizeof(block));    // Reset block
                blockPtr = -1;                      // Reset blockPtr
                op = true;
            }
        }

        // If no operation has been done then there is no valid match for the given character
        if (!op)
        {
            printf("Bad Input - Invalid number");
            free(number);
            return NULL;
        }
    }

    // If block is not zeroed out, final chunk is yet to be appended to number
    if (
        block[0] != 0 || block[1] != 0 ||
        block[2] != 0 || value != 0
    ) {
        block[3] = value;

        for (int k = 0; k < 4; k++)
        {
            number[48 + k] = '0' + block[k];
        }
    }

    int offset = -1;
    while (number[++offset] == '0');    // Strip leading zeroes
    memmove(number, number + offset, 53 - offset);

    return number;
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