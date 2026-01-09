/***
 *      Roman numbers
 *      Given a number, return its successor in Roman Nuemrals.
 * 
 *      ___Working__Procedure___
 *      ->  The executable is called with a single parameter.
 *      ->  The parameter can either be a numerical number or a Roman Numeral.
 *      ->  If the parameter is a Roman Numeral, it is first parsed using toNumber() to identify its equivalent.
 *      ->  The Roman Numeral format of the next number is returned by toRoman(number + 1).
 *      ->  Invalid input cases will be handled.
 * 
 *      ___Input__Requirements___
 *      ->  The inputted number can be a positive integer in the range [0, 1000000000).
 *      ->  If a Roman Number is inputted, it should use the overline vinculum notation supported by this code.
 *          (WILL BE DOCUMENTED IN DETAIL)
 *      ->  For numeric parameter, non-digit characters should not be used.
 *      ->  For Roman Numeral parameter, non-Roman numeral characters should not be used.
 * 
 *      ___NOTE___
 *      ->  Windows does not render overlines in its Powershell terminal/command prompt.
 *      ->  This is an OS bug, not an implementation error.
 *      ->  Display might not work, but the characters will correctly be copied to the clipboard.
 *      ->  For numbers above 4000, users are discouraged from using Roman Numeral parameter as it returns true -ves.
 */


#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include <fcntl.h>
#include <io.h>


// Overline ASCII : 773
static wchar_t VINCULUM = (wchar_t) 773;
static char NUMERALS[3][3] = {
    {'M', 'V', 'X'},
    {'X', 'L', 'C'},
    {'C', 'D', 'M'}
};


int toNumber(wchar_t *);
wchar_t *toRoman(int);


int toNumber(wchar_t *roman) {
    int n = wcslen(roman);
    int number = 0, vinculumCount = 0;

    if (n > 80) {
        wprintf(L"Bad Input: Input too large\n");
        return -1;
    }

    // Traverse in reverse for easier counting
    for (int i = n-1; i >= 0; i--) {
        int baseValue = 1;
        if ((wint_t) roman[i] == 773 && ++vinculumCount >= 3) {
            wprintf(L"Bad Input: Too many vinculums\n");
            return -1;
        } else switch(roman[i]) {
            case L'I':
                if (vinculumCount > 0) {
                    wprintf(L"Bad Input: Invalid Numeral\n");
                    return -1;
                }

                if (number == 5 || number == 10) {
                    --number;
                } else if (number < 3) {
                    ++number;
                } else {
                    wprintf(L"Bad Input: Invalid Numeral\n");
                    return -1;
                }
            break;

            
            case L'M':
                baseValue *= 10;

            case L'C':
                baseValue *= 10;

            case L'X':
                baseValue *= 10;

                for (int j = 0; j < vinculumCount; j++) {
                    baseValue *= 1000;
                }

                if (number < baseValue) {
                    number += baseValue;
                } else if ((number >= 5 * baseValue && number < 6 * baseValue) || 
                           (number >= 10 * baseValue && number < 11 * baseValue)) {
                    number -= baseValue;
                } else if (number < 3 * baseValue) {
                    number += baseValue;
                } else {
                    wprintf(L"Bad Input: Invalid Numeral\n");
                    return -1;
                }

                vinculumCount = 0;
            break;
            

            case L'D':
                baseValue *= 10;

            case L'L':
                baseValue *= 10;

            case L'V':
                baseValue *= 5;

                for (int j = 0; j < vinculumCount; j++) {
                    baseValue *= 1000;
                }
                
                if (number < 4 * baseValue / 5) {
                    number += baseValue;
                } else {
                    wprintf(L"Bad Input: Invalid number\n");
                    return -1;
                }

                vinculumCount = 0;
            break;

            default:
                wprintf(L"Bad Input: Invalid Number\n");
                return -1;
            break;
        }
    }

    if (vinculumCount != 0) {
        wprintf(L"Bad Input: Invalid Numeral\n");
        return -1;
    }

    return number;
}


wchar_t *toRoman(int number) {
    if (number >= 1000000000) {
        wprintf(L"Bad Input: Number too large\n");
        return NULL;
    }

    if (number <= 0) {
        wprintf(L"Bad Input: Non-positive number\n");
        return NULL;
    }

    wchar_t *roman = (wchar_t *)malloc(80 * sizeof(wchar_t));
    int copy = number, cursor = 0;

    // Generate number in reverse order
    // Then reverse order of characters

    for (int i = 0; copy != 0; i++) {
        int digit = copy % 10;
        switch(digit) {
            case 0:
                break;

            case 1:
            case 2:
            case 3:
            case 6:
            case 7:
            case 8:
                if (i == 0) {
                    for (int j = 0; j < (digit % 5); j++) {
                        roman[cursor++] = L'I';
                    }
                } else {
                    for (int j = 0; j < (digit % 5); j++) {
                        for (int k = (i % 3 == 0) ? 1 : 0; 
                                 k < i / 3; k++) {
                            roman[cursor++] = VINCULUM;
                        }
                        roman[cursor++] = (wchar_t) NUMERALS[i % 3][0];
                    }
                }
                if (digit <= 3) break;
            
            case 4:
            case 5:
                for (int j = 0; j < i / 3; j++) {
                    roman[cursor++] = VINCULUM;
                }
                roman[cursor++] = (wchar_t) NUMERALS[i % 3][1];
                if (digit != 4) break;

            case 9:
                if (digit == 9) {
                    for (int j = 0; j < i / 3; j++) {
                        roman[cursor++] = VINCULUM;
                    }
                    roman[cursor++] = (wchar_t) NUMERALS[i % 3][2];
                }

                if (i == 0) {
                    roman[cursor++] = L'I';
                } else {
                    for (int k = (i % 3 == 0) ? 1 : 0; 
                             k < i / 3; k++) {
                        roman[cursor++] = VINCULUM;
                    }
                    roman[cursor++] = (wchar_t) NUMERALS[i % 3][0];
                }
            ;
        }
        copy /= 10;
    }

    roman[cursor] = L'\0';

    // Reverse order
    for (int left = 0, right = cursor - 1; left < right; left++, right--) {
        wchar_t temp = roman[left];
        roman[left] = roman[right];
        roman[right] = temp;
    }

    return roman;
}


int main(int argc, char *argv[]) {
    /**
     *      ___Command__Line__Syntax___
     *          .\roman.exe [option-flag] <number>
     *          
     *      [] - optional flag
     *      <> - required argument
     * 
     *      ___Option__Flags____
     *      NOTE: If omitted, option defaults to -convert
     *          -c / -convert : Converts given number to Roman number or vice versa
     *          -n / -next    : Returns next Roman number
     */
    

    if (argc < 2) {
        printf("Invalid Syntax: Check docstring for command line syntax");
        return -1;
    }

    int number, x = 1;
    char option = 'c';
    wchar_t *roman = NULL;
    clock_t startTime, endTime;

    _setmode(_fileno(stdout), _O_U16TEXT);
    setlocale(LC_ALL, "");
    startTime = clock();

    if (argv[x][0] == '-') {
        if (
            strcmp(argv[x], "-c") == 0 || 
            strcmp(argv[x], "-convert") == 0
        );  // Default argument
        else if (
            strcmp(argv[x], "-n") == 0 ||
            strcmp(argv[x], "-next") == 0
        ) { option = 'n'; }
        else {
            wprintf(L"Invalid Syntax: Check docstring for command line syntax");
            return -1;
        }
        ++x;
    }

    if (isdigit(argv[x][0])) {
        number = 0;
        int n = strlen(argv[x]);
        for (int i = 0; i < n; i++) {
            if (number >= 1000000000) {
                wprintf(L"Bad Input: Number too big\n");
                return -1;
            }

            if (!isdigit(argv[x][i])) {
                wprintf(L"Bad Input: Invalid number\n");
                return -1;
            }

            int digit = argv[x][i] - '0';
            number = (number * 10) + digit;
        }

        // roman = toRoman(number + 1);
        switch(option) {
            case 'c':
                roman = toRoman(number);
            break;

            case 'n':
                roman = toRoman(number + 1);
            break;
        }
    } else {
        size_t len = mbstowcs(NULL, argv[x], 0);
        if (len == (size_t) -1) {
            wprintf(L"Bad Input: Could not convert to normal input form\n");
            return -1;
        }

        roman = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
        mbstowcs(roman, argv[x], len + 1);

        number = toNumber(roman);

        free(roman);
        roman = NULL;

        if (number != -1 && option == 'n') {
            roman = toRoman(number + 1);
        }
    }

    endTime = clock();

    if (argc > x + 1) {
        wprintf(L"WARNING: Extra arguments will be ignored...\n\n");
    }

    switch(option) {
        case 'c':
            if (roman != NULL) {
                wprintf(L"%ls\n", roman);
            } else if (number != -1) {
                wprintf(L"%d\n", number);
            }
        break;

        case 'n':
            if (roman != NULL) {
                wprintf(L"Next Number: %ls\n", roman);
            }
        break;
    }

    if (roman != NULL) free(roman);

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    wprintf(L"\nExecution time: %.4lf seconds", timeUsed);

    return 0;
}