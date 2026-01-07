/**
 *      Palindrome
 *      Given a palindromic number, finds the next palindromic number
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


bool isPalindrome(char *number) {
    int n = strlen(number);
    if (number[n - 1] == '0') return false;

    for (
        char *left = number, *right = number + n - 1; 
        left < right; 
        left++, right--
    ) {
        if (!isdigit(*left) || *left != *right) {
            return false;
        }
    }
    return true;
}


char *nextPalindrome(char *number) {
    int n = strlen(number);
    char *nextNumber = (char *)malloc((n + 2) * sizeof(char));
    strcpy(nextNumber, number);

    for (
        int left = (n-1)/2, right = n/2;
        left >= 0 && right < n;
        left--, right++
    ) {
        if (nextNumber[left]++ == '9') {
            nextNumber[left] = '0';
            nextNumber[right] = '0';
            continue;
        } else if (left != right) {
            nextNumber[right]++;
        } break;
    }

    if (nextNumber[0] == '0') {
        nextNumber[0]++;
        nextNumber[n] = '1';
        nextNumber[n+1] = '\0';
    }

    return nextNumber;
}


int main(int argc, char *argv[]) {
    /**
     *      ___Command__Line__Syntax___
     *          .\palindrome.exe <number>
     */

    if (argc < 2) {
        perror("Invalid Syntax: .\\palindrome.exe <number>");
        return -1;
    }

    int offset = 0;     // To skip leading zeroes
    char *nextNumber;
    clock_t startTime, endTime;

    startTime = clock();

    for (; argv[1][offset] == '0'; offset++) {
        if (argv[1][offset] == '\0') {
            // Entire number is just zeroes
            --offset;
            break;
        }
    }

    if (!isPalindrome(argv[1] + offset)) {
        printf("Bad Input - Not a palindromic number");
        return -1;
    }

    nextNumber = nextPalindrome(argv[1] + offset);
    endTime = clock();

    if (argc > 2) {
        printf("WARNING: Extra arguments will be ignored...\n\n");
    }

    printf("%s\n", nextNumber);
    free(nextNumber);

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4lf seconds", timeUsed);

    return 0;
}