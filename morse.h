/**
 *      Morse Code Counting
 *      Clusters of five per digit
 *      Digit clusters are separated by spaces.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


const char *MORSE_CODE_DIGITS[10] = 
{
    "-----",        // 0
    ".----",        // 1
    "..---",        // 2
    "...--",        // 3
    "....-",        // 4
    ".....",        // 5
    "-....",        // 6
    "--...",        // 7
    "---..",        // 8
    "----."         // 9
};


char *translate_to_morse_code(char *number)
{
    int n = strlen(number);

    char *morseNumber = (char *)malloc(6 * n * sizeof(char));
    if (!morseNumber)
    {
        printf("Error - malloc failure");
        return NULL;
    }

    int morseCodePointer = 0;
    for (int i = 0; i < n; i++)
    {
        if (!isdigit(number[i]))
        {
            printf("Bad Input - Invalid number");
            free(morseNumber);
            return NULL;
        }

        if (
            morseCodePointer == 0 && 
            number[i] == '0' && 
            i < n - 1
        ) { continue; }     // Removing leading zeroes

        memcpy(morseNumber + morseCodePointer, MORSE_CODE_DIGITS[number[i] - '0'], 5);
        morseNumber[morseCodePointer + 5] = ' ';
        morseCodePointer += 6;
    }
    morseNumber[morseCodePointer - 1] = '\0';

    return morseNumber;
}


char *translate_from_morse_code(char *morseNumber)
{
    int n = strlen(morseNumber);

    char *number = (char *)malloc(n/4 * sizeof(char));
    if (!number)
    {
        printf("Error - malloc failure");
        return NULL;
    }

    int numPointer = 0;
    char *digit = strtok(morseNumber, " ");
    while (digit)
    {
        if (strlen(digit) != 5)
        {
            printf("Bad Input - Illegal morse code");
            free(number);
            return NULL;
        }

        if (digit[0] == '.')
        {
            int i, j, k = 0;

            for (i = 0; i < 5; i++)
            {
                if (digit[i] == '.')
                    ++k;
                else if (digit[i] == '-')
                    break;
                else
                {
                    printf("Bad Input - Illegal morse code");
                    free(number);
                    return NULL;
                }
            }

            for (j = i + 1; j < 5; j++)
            {
                if (digit[j] != '-')
                {
                    printf("Bad Input - Illegal morse code");
                    free(number);
                    return NULL;
                }
            }

            number[numPointer++] = '0' + k;
        }
        else if (digit[0] == '-')
        {
            int i, j, k = 5;

            for (i = 0; i < 5; i++)
            {
                if (digit[i] == '-')
                    ++k;
                else if (digit[i] == '.')
                    break;
                else
                {
                    printf("Bad Input - Illegal morse code");
                    free(number);
                    return NULL;
                }
            }

            for (j = i + 1; j < 5; j++)
            {
                if (digit[j] != '.')
                {
                    printf("Bad Input - Illegal morse code");
                    free(number);
                    return NULL;
                }
            }

            number[numPointer++] = '0' + (k % 10);
        }
        else
        {
            printf("Bad Input - Illegal morse code");
            free(number);
            return NULL;
        }

        digit = strtok(NULL, " ");
    }

    number[numPointer] = '\0';

    // Skip leading zeroes
    int offset = -1;
    while (number[++offset] == '0' && offset < numPointer - 1);
    memmove(number, number + offset, numPointer - offset + 1);

    return number;
}