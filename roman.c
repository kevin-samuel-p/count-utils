#include "roman.h"

/**
 *      Roman Numbers
 *      Given a number, return its successor in Roman Numerals
 * 
 *      NOTE: For number_to_roman, regardless of representation,
 *            take a round-trip by feeding the output to roman_to_number()
 * 
 *            if (strcmp(number, "4000") < 0)
 *                return number_to_roman(number, 'i');
 * 
 *      Ensure to strip leading zeroes beforehand
 */


#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <ctype.h>


// Overline ASCII = 733
static wchar_t VINCULUM = (wchar_t) 773;
static const char NUMERALS[3][3] = 
{
    {'I', 'V', 'X'},
    {'X', 'L', 'C'},
    {'C', 'D', 'M'}
};


char *roman_to_number(const wchar_t *roman)
{
    /**
     *      ->  Count the number of vinculums in the highest digit
     *      ->  Malloc an int array to store chunks of thousands
     *      ->  Serialize chunks as final string
     */

    int n = wcslen(roman);
    int vinculumCount = 0;

    int *chunks, size = 1, lastChunk = 0, i;
    char baseValue;

    switch(roman[0])
    {
        case L'M':
            ++size;
        case L'D':
        case L'C':
        case L'L':
        case L'X':
        case L'V':
        case L'I':
            baseValue = (char)roman[0];
        break;

        default:
            printf("Bad Input - Invalid Roman numeral");
            return NULL;
    }

    for (
        i = 1; 
        i < n && roman[i] == VINCULUM; 
        i++, size++, vinculumCount++
    );

    chunks = (int *)calloc(size, sizeof(int)); 
    if (!chunks)
    {
        printf("Error - calloc failure");
        return NULL;
    }
    
    for (; i <= n; i++)
    {
        if (i != n && roman[i] == VINCULUM)
        {
            ++vinculumCount;
        }
        else
        {
            int index = size - vinculumCount - 1;

            if (index < lastChunk)
            {
                printf("Bad Input - Invalid Roman numeral");
                free(chunks);
                return NULL;
            }

            switch(baseValue)
            {
                case 'M':
                    if (chunks[index] % 1000 == 100)
                    {
                        chunks[index] += 800;
                        lastChunk = index;
                    }
                    else if (index == 0)
                    {
                        printf("Bad Input - Invalid Roman numeral");
                        free(chunks);
                        return NULL;
                    }
                    else switch(chunks[index - 1] % 10)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 5:
                        case 6:
                        case 7:
                            chunks[index - 1] += 1;
                            lastChunk = index - 1;
                        break;

                        default:
                            printf("Bad Input - Invalid Roman numeral");
                            free(chunks);
                            return NULL;
                    }
                break;

                case 'D':
                    switch(chunks[index] % 1000)
                    {
                        case 0:
                            chunks[index] += 500;
                        break;

                        case 100:
                            chunks[index] += 300;
                        break;

                        default:
                            printf("Bad Input - Invalid Roman numeral");
                            free(chunks);
                            return NULL;
                    }
                    lastChunk = index;
                break;

                case 'C':
                    if (chunks[index] % 100 == 10)
                    {
                        chunks[index] += 80;
                    }
                    else switch(chunks[index] % 1000)
                    {
                        case 0:
                        case 100:
                        case 200:
                        case 500:
                        case 600:
                        case 700:
                            chunks[index] += 100;
                        break;

                        default:
                            printf("Bad Input - Invalid Roman numeral");
                            free(chunks);
                            return NULL;
                    }
                    lastChunk = index;
                break;

                case 'L':
                    switch(chunks[index] % 100)
                    {
                        case 0:
                            chunks[index] += 50;
                        break;

                        case 10:
                            chunks[index] += 30;
                        break;

                        default:
                            printf("Bad Input - Invalid Roman numeral");
                            free(chunks);
                            return NULL;
                    }
                    lastChunk = index;
                break;

                case 'X':
                    if (chunks[index] % 10 == 1)
                    {
                        chunks[index] += 8;
                    }
                    else switch(chunks[index] % 100)
                    {
                        case 0:
                        case 10:
                        case 20:
                        case 50:
                        case 60:
                        case 70:
                            chunks[index] += 10;
                        break;

                        default:
                            printf("Bad Input - Invalid Roman numeral");
                            free(chunks);
                            return NULL;
                    }
                    lastChunk = index;
                break;

                case 'V':
                    switch(chunks[index] % 10)
                    {
                        case 0:
                            chunks[index] += 5;
                        break;

                        case 1:
                            chunks[index] += 3;
                        break;

                        default:
                            printf("Bad Input - Invalid Roman numeral");
                            free(chunks);
                            return NULL;
                    }
                    lastChunk = index;
                break;

                case 'I':
                    switch(chunks[index] % 10)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 5:
                        case 6:
                        case 7:
                            chunks[index] += 1;
                        break;

                        default:
                            printf("Bad Input - Invalid Roman numeral");
                            free(chunks);
                            return NULL;
                    }
                    lastChunk = index;
                break;
            }

            vinculumCount = 0;
            switch(roman[i])
            {
                case L'M':
                case L'D':
                case L'C':
                case L'L':
                case L'X':
                case L'V':
                case L'I':
                    baseValue = (char)roman[i];
                case L'\0':
                
                break;

                default:
                    printf("Bad Input - Invalid Roman numeral");
                    free(chunks);
                    return NULL;
            }
        }
    }

    char *number = (char *)malloc((3*size + 2) * sizeof(char));
    if (!number)
    {
        printf("Error - malloc failure");
        free(chunks);
        return NULL;
    }

    char chunk[4];
    number[0] = '\0';
    for (int i = 0; i < size; i++)
    {
        sprintf(chunk, (i == 0) ? "%d" : "%03d", chunks[i]);
        strcat(number, chunk);
    }

    free(chunks);

    return number;
}


wchar_t *number_to_roman(const char *number, char representation)
{
    if (number[0] == '-')
    {
        printf("Bad Input - Non-positive numbers not allowed");
        return NULL;
    }

    int n = strlen(number);
    int currSize = 80;
    wchar_t *roman = (wchar_t *)malloc(currSize * sizeof(wchar_t));   // Expand size dynamically
    if (!roman)
    {
        printf("Error - malloc failure");
        return NULL;
    }
    
    int romanPtr = 0;

    // Generate number in reverse order
    // Then reverse order of characters

    for (int i = 0; i < n; i++)
    {
        char digit = number[n - i - 1];
        switch(digit)
        {
            case '0':
                break;

            case '1':
            case '2':
            case '3':
            case '6':
            case '7':
            case '8':

                for (int j = 0; j < (digit - '0') % 5; j++)
                {
                    for (int k = 0; k < i / 3; k++) 
                    {
                        roman[romanPtr++] = VINCULUM;
                        if (romanPtr == currSize - 2)
                        {
                            currSize += 80;
                            wchar_t *temp = (wchar_t *)realloc(roman, currSize * sizeof(wchar_t));
                            if (!temp)
                            {
                                printf("Error - realloc failure");
                                free(roman);
                                return NULL;
                            }
                            roman = temp;
                        }
                    }
                    
                    roman[romanPtr++] = (wchar_t)NUMERALS[i % 3][0];
                    if (romanPtr == currSize - 2)
                    {
                        currSize += 80;
                        wchar_t *temp = (wchar_t *)realloc(roman, currSize * sizeof(wchar_t));
                        if (!temp)
                        {
                            printf("Error - realloc failure");
                            free(roman);
                            return NULL;
                        }
                        roman = temp;
                    }
                }
            
                if (digit <= '3')
                    break;

            case '4':
            case '5':
                for (int j = 0; j < i / 3; j++)
                {
                    roman[romanPtr++] = VINCULUM;
                    if (romanPtr == currSize - 2)
                    {
                        currSize += 80;
                        wchar_t *temp = (wchar_t *)realloc(roman, currSize * sizeof(wchar_t));
                        if (!temp)
                        {
                            printf("Error - realloc failure");
                            free(roman);
                            return NULL;
                        }
                        roman = temp;
                    }
                }

                roman[romanPtr++] = (wchar_t)NUMERALS[i % 3][1];
                if (romanPtr == currSize - 2)
                {
                    currSize += 80;
                    wchar_t *temp = (wchar_t *)realloc(roman, currSize * sizeof(wchar_t));
                    if (!temp)
                    {
                        printf("Error - realloc failure");
                        free(roman);
                        return NULL;
                    }
                    roman = temp;
                }

                if (digit != '4')
                    break;

            case '9':
                if (digit == '9')
                {
                    for (int j = 0; j < i / 3; j++)
                    {
                        roman[romanPtr++] = VINCULUM;
                        if (romanPtr == currSize - 2)
                        {
                            currSize += 80;
                            wchar_t *temp = (wchar_t *)realloc(roman, currSize * sizeof(wchar_t));
                            if (!temp)
                            {
                                printf("Error - realloc failure");
                                free(roman);
                                return NULL;
                            }
                            roman = temp;
                        }
                    }

                    roman[romanPtr++] = (wchar_t)NUMERALS[i % 3][2];
                    if (romanPtr == currSize - 2)
                    {
                        currSize += 80;
                        wchar_t *temp = (wchar_t *)realloc(roman, currSize * sizeof(wchar_t));
                        if (!temp)
                        {
                            printf("Error - realloc failure");
                            free(roman);
                            return NULL;
                        }
                        roman = temp;
                    }
                }


                for (int k = 0; k < i / 3; k++) 
                {
                    roman[romanPtr++] = VINCULUM;
                    if (romanPtr == currSize - 2)
                    {
                        currSize += 80;
                        wchar_t *temp = (wchar_t *)realloc(roman, currSize * sizeof(wchar_t));
                        if (!temp)
                        {
                            printf("Error - realloc failure");
                            free(roman);
                            return NULL;
                        }
                        roman = temp;
                    }
                }

                roman[romanPtr++] = (wchar_t)NUMERALS[i % 3][0];
                if (romanPtr == currSize - 2)
                {
                    currSize += 80;
                    wchar_t *temp = (wchar_t *)realloc(roman, currSize * sizeof(wchar_t));
                    if (!temp)
                    {
                        printf("Error - realloc failure");
                        free(roman);
                        return NULL;
                    }
                    roman = temp;
                }
            break;

            default:
                printf("Bad Input - Invalid number");
                free(roman);
                return NULL;
        }
    }

    // If number is zero, all iterations would have been skipped
    if (romanPtr == 0)
    {
        printf("Bad Input - Non-positive numbers not allowed");
        free(roman);
        return NULL;
    }

    roman[romanPtr] = L'\0';

    // Reverse order
    for (int left = 0, right = romanPtr - 1; left < right; left++, right--)
    {
        wchar_t temp = roman[left];
        roman[left] = roman[right];
        roman[right] = temp;
    }

    // Final touchups: default is legal representation
    if (representation == 'i')  // Idiomatic representation replacing I̅ with M
    {
        wchar_t *replacer = wcsstr(roman, L"I̅");
        while (replacer)
        {
            *replacer = L'M';

            size_t tailLen = (roman + romanPtr) - (replacer + 2);
            memmove(replacer + 1, replacer + 2, tailLen * sizeof(wchar_t));
            
            --romanPtr;
            replacer = wcsstr(roman, L"I̅");
        }

        roman[romanPtr] = L'\0';
    }

    return roman;
}