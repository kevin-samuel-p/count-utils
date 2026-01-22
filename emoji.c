/**
 *      Emoji
 *      Function to emojify numbers
 */

#include "emoji.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


const char * const EMOJI_DIGITS[] = 
{
    "0️⃣", "1️⃣", "2️⃣", "3️⃣", "4️⃣",
    "5️⃣", "6️⃣", "7️⃣", "8️⃣", "9️⃣"
};


char *number_to_emoji(const char *number)
{
    if (number[0] == '-')
    {
        printf("Bad Input - Negative numbers not allowed");
        return NULL;
    }

    int n = strlen(number);
    
    char *res = (char *)malloc((7*n + 1) * sizeof(char));
    if (!res)
    {
        printf("Error - malloc failure");
        return NULL;
    }
    res[0] = '\0';

    for (int i = 0; i < n; i++) 
    {
        // Catch invalid inputs
        if (!isdigit((unsigned char)number[i]))
        {
            printf("Bad Input - Invalid number");
            free(res);
            return NULL;
        }

        strcat(res, EMOJI_DIGITS[number[i] - '0']);
    }

    return res;
}