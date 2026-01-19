/**
 *      Emoji
 *      Function to emojify numbers
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


const char *EMOJI_DIGITS[10] = 
{
    "0️⃣", "1️⃣", "2️⃣", "3️⃣", "4️⃣",
    "5️⃣", "6️⃣", "7️⃣", "8️⃣", "9️⃣"
};


// Emojifies number, only accepts inputs upto 1 billion
char *numberToEmoji(char *number)
{
    if (number[0] == '-')
    {
        printf("Bad Input - Negative numbers not allowed");
        return NULL;
    }

    char *res = (char *)malloc(64 * sizeof(char));
    if (!res)
    {
        printf("Error - malloc failure");
        return NULL;
    }
    res[0] = '\n';

    for (int i = 0; number[i] != '\0'; i++) 
    {
        // Catch invalid inputs
        if (!isdigit(number[i]))
        {
            printf("Bad Input - Invalid number");
            free(res);
            return NULL;
        }

        // Catch extra large numbers
        // Only supports numbers < 1 billion
        if (i >= 9)
        {
            printf("Bad Input - Number too large");
            free(res);
            return NULL;
        }

        strcat(res, EMOJI_DIGITS[number[i]]);
    }

    return res;
}