/**
 *      Roman Numbers
 *      Given a number, return its successor in Roman Numerals
 */


#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <ctype.h>


// Overline ASCII = 733
static wchar_t VINCULUM = (wchar_t) 773;
static char NUMERALS[3][3] = 
{
    {'M', 'V', 'X'},
    {'X', 'L', 'C'},
    {'C', 'D', 'M'}
};


char *roman_to_number(wchar_t *);
wchar_t *number_to_roman(char *);


char *roman_to_number(wchar_t *roman)
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


