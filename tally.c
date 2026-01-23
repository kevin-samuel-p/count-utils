#include "tally.h"

/**
 *      Tally Marks
 *      
 *      Format:
 *      x(.5)K + (\n)
 *      ~~||||~~ ~~||||~~ ~~||||~~ ~~||||~~ .... |||
 * 
 *      Double tilde blocks used for formatting strikethrough text on Discord.
 *      Pipes | used to represent tallies.
 *      Optional -# string as a tiny text formatter will be seen, must be ignored while parsing.
 *      Same goes for even rarer # big text formatter.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


const char * FIVE_BAR = "~~||||~~";
const char * FIFTY_LINE = 
    "~~||||~~ ~~||||~~ ~~||||~~ ~~||||~~ ~~||||~~ "
    "~~||||~~ ~~||||~~ ~~||||~~ ~~||||~~ ~~||||~~";


int parse_tally_marks(char *textWall)
{
    int number = 0,     // tracks the number while parsing text wall
        parseCount;
    float k = 0;        // stores thousand count after parsing header, if it exists
    char *chunk, parseBuffer[2] = {0};
    char delimiter[] = " \n+";    // Remove + from delimiter after parsing header

    chunk = strtok(textWall, delimiter);
    if (!chunk)
    {
        // User almost found a vulnerability! + is rogue input.
        printf(
            "Bad Input - Invalid format\n\n"
        
            "{x(.5)}K + [tally marks]"
        );
        return -1;
    }
    
    /**
     *  Header possible cases:
     *      <empty header>  ->  nothing gets parsed
     *      %f(k/K) +       ->  gets parsed correctly
     *      %f (k/K) +      ->  needs two strtok() calls   
     */
    parseCount = sscanf(chunk, "%f%1s", &k, parseBuffer);
    switch(parseCount)
    {
        case 0:
            break;

        case 1:
            chunk = strtok(NULL, delimiter);
            if (!chunk)
            {
                printf(
                    "Bad Input - Invalid format\n\n"
                
                    "{x(.5)}K + [tally marks]"
                );
                return -1;
            }
            parseCount = sscanf(chunk, "%1s", parseBuffer);     // Fall through

        case 2:
            if (tolower(parseBuffer[0]) == 'k')
            {
                chunk = strtok(NULL, delimiter);
                delimiter[2] = '\0';    // Removing + from delimiter
                break;
            }

        default:
            printf(
                "Bad Input - Invalid format\n\n"
            
                "{x(.5)}K + [tally marks]"
            );  

        return -1;
    }

    delimiter[2] = '\0';    // Removing + from delimiter after parsing header
    number = (int)(k * 1000 + 0.5f);
    if (number % 500 != 0)
    {
        printf("Bad Input - %f is a bad marker", k); 
        return -1;
    }
    
    while (chunk)
    {
        if (strcmp(chunk, FIVE_BAR) == 0)
        {
            number += 5;
            chunk = strtok(NULL, delimiter);
            continue;
        }

        // Check for Discord markdown formatters
        if (
            strcmp(chunk, "#") == 0 || 
            strcmp(chunk, "##") == 0 || 
            strcmp(chunk, "###") == 0 ||
            strcmp(chunk, "-#") == 0
        ) {
            chunk = strtok(NULL, delimiter);
            continue;
        }

        int chunkSize = strlen(chunk);
        if (chunkSize > 4)
        {
            printf("Bad Input - %s is not a valid block", chunk);
            return -1;
        }

        for (int i = 0; i < chunkSize; i++)
        {
            if (chunk[i] != '|')
            {
                printf("Bad Input - %s is not a valid block", chunk);
                return -1;
            }
            ++number;
        }

        chunk = strtok(NULL, delimiter);
        if (chunk)
        {
            printf("Bad Input - Invalid group arrangement");
            return -1;
        }

        break;
    }

    return number;
}


char *tally(int number, enum Formatting formatting)
{
    if (number <= 0)
    {
        printf("Bad Input - Tally marks cannot represent non-positive numbers");
        return NULL;
    }
    
    char *slab, header[20], body[1980], *prefix;
    int headerPtr = 0, bodyPtr = 0;

    slab = (char *)malloc(2000 * sizeof(char));
    if (!slab)
    {
        printf("Error - malloc failure");
        return NULL;
    }

    switch(formatting)
    {
        case BIG:
            prefix = "\n## ";
        break;

        case COMPACT:
            prefix = " ";
        break;

        case GIANT:
            prefix = "\n# ";
        break;

        case IMPACT:
            prefix = "\n### ";
        break;

        case TINY_TEXT:
            prefix = "\n-# ";
        break;

        case DEFAULT:
            prefix = "\n";
        break;
    }

    if (number >= 1000)
    {
        headerPtr += snprintf(header, sizeof(header), "%d", number / 1000);
        number %= 1000;

        if (number >= 500)
        {
            strcpy(header + headerPtr, ".5");
            headerPtr += 2;
            number -= 500;
        }

        strcpy(header + headerPtr, "K");
        headerPtr += 1;
    }
    else
    {
        header[0] = '\0';
    }

    for (; number >= 50; number -= 50)
    {
        bodyPtr += snprintf(
            body + bodyPtr, 
            sizeof(body) - bodyPtr, 
            "%s%s", 
            prefix, FIFTY_LINE
        );
    }

    if (number > 0)
    {
        bodyPtr += snprintf(
            body + bodyPtr, 
            sizeof(body) - bodyPtr, 
            "%s", 
            prefix
        );
    }

    for (; number >= 5; number -= 5)
    {
        bodyPtr += snprintf(
            body + bodyPtr, 
            sizeof(body) - bodyPtr, 
            "%s%c", 
            FIVE_BAR, ' '
        );
    }

    for (; number > 0; number--)
    {
        body[bodyPtr++] = '|';
    }
    body[bodyPtr] = '\0';

    // Attempt to mitigate overallocation
    char *temp = realloc(slab, headerPtr + bodyPtr + 10);   // 10 just to be safe
    if (!temp)
    {
        printf("Warning - realloc failure\n");
    }
    else
    {
        slab = temp;
    }

    const char *separator = (
        header[0] && body[0]
    ) ? " +" : "";

    sprintf(
        slab, 
        "%s%s%s", 
        header, separator, body
    );

    return slab;
}