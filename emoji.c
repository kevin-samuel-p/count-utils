/**
 *      Emoji
 *      Given a number, return the emojified version of that number
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>


const char *EMOJI_DIGITS[10] = {
    "0️⃣", "1️⃣", "2️⃣", "3️⃣", "4️⃣",
    "5️⃣", "6️⃣", "7️⃣", "8️⃣", "9️⃣"
};


//  Emojifies number, only accepts inputs upto 1 billion
char *numberToEmoji(int number) {
    char numberString[10];
    char *res = (char *)malloc(71 * sizeof(char));
    res[0] = '\0';
    
    sprintf(numberString, "%d", number);

    int n = strlen(numberString);
    for (int k = 0; k < n; k++) {
        strcat(res, EMOJI_DIGITS[numberString[k] - '0']);
    }

    return res;
}


int main(int argc, char *argv[]) {
    /**
     *      ___Command__Line__Syntax___
     *          .\emoji.exe [function-flag] <number>
     * 
     *      <> - necessary argument(s)
     *      [] - optional flag(s)
     * 
     *      ___Function__Flags___
     *      OPTION: If omitted, it assummed to be -c flag by default.
     *          -c / -convert : Emojifies the given number
     *          -n / -next    : Returns the next number emojified
     */

    if (argc < 2) {
        printf("Invalid Syntax: .\\emoji.exe [function-flag] <number>");
        return -1;
    }

    setlocale(LC_ALL, ".UTF-8");
    system("chcp 65001 > nul");

    int x = 1, number = 0;  // Index for numeral argument (to track extra arguments in command line call) + var for number
    char function = 'c';    // Flag for type of operation (simple emojification vs next number)
    char *emojifiedNumber;
    clock_t startTime, endTime;

    startTime = clock();
    
    if (argv[1][0] == '-') {
        if (
            strcmp(argv[1], "-c") == 0 || 
            strcmp(argv[1], "-convert") == 0
        ) { ++x; }
        else if (
            strcmp(argv[1], "-n") == 0 || 
            strcmp(argv[1], "-next") == 0
        ) {
            function = 'n';
            ++x;
        } else if (argc > 2) {
            printf("Invalid Syntax: .\\emoji.exe [function-flag] <number>");
            return -1;
        }
    }

    if (argc <= x) {
        printf("Invalid Syntax: .\\emoji.exe [function-flag] <number>");
        return -1;
    }

    int n = strlen(argv[x]);
    for (int i = 0; i < n; i++) {
        if (!isdigit(argv[x][i])) {
            printf("Bad Input - Invalid number");
            return -1;
        }

        if (number >= 100000000) {
            // Maximum allowed number for this implementation is upto 1 billion
            printf("Bad Input - Number too large");
            return -1;
        }
        number = (number * 10) + (argv[x][i] - '0');
    }

    switch(function) {
        case 'c':
            emojifiedNumber = numberToEmoji(number);
        break;

        case 'n':
            emojifiedNumber = numberToEmoji(number + 1);
        break;
    }

    endTime = clock();

    if (argc > x + 1) {
        printf("WARNING: Extra arguments will be ignored...\n\n");
    }

    printf("%s\n", emojifiedNumber);
    free(emojifiedNumber);

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4lf seconds", timeUsed);

    return 0;
}