#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>


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
        printf("Bad Input: Input too large\n");
        return -1;
    }

    // Traverse in reverse for easier counting
    for (int i = n-1; i >= 0; i--) {
        int baseValue = 1;
        if ((wint_t) roman[i] == 773 && ++vinculumCount >= 3) {
            printf("Bad Input: Too many vinculums\n");
            return -1;
        } else switch(roman[i]) {
            case L'I':
                if (vinculumCount > 0) {
                    printf("Bad Input: Invalid Numeral\n");
                    return -1;
                }

                if (number == 5 || number == 10) {
                    --number;
                } else if (number < 3) {
                    ++number;
                } else {
                    printf("Bad Input: Invalid Numeral\n");
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
                    printf("Bad Input: Invalid Numeral\n");
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
                    printf("Bad Input: Invalid Number\n");
                    return -1;
                }

                vinculumCount = 0;
            break;

            default:
                printf("Bad Input: Invalid Number\n");
                return -1;
            break;
        }
    }

    if (vinculumCount != 0) {
        printf("Bad Input: Invalid Numeral\n");
        return -1;
    }

    return number;
}


wchar_t *toRoman(int number) {
    if (number >= 1000000000) {
        printf("Bad Input: Number too large\n");
        return NULL;
    }

    if (number <= 0) {
        printf("Bad Input: Non-positive number\n");
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
    if (argc != 2) {
        perror("Invalid Syntax: .\\norep.exe [number/roman]");
        return -1;
    }

    int number;
    wchar_t *roman = NULL;

    if (isdigit(argv[1][0])) {
        number = 0;
        int n = strlen(argv[1]);
        for (int i = 0; i < n; i++) {
            if (number >= 1000000000) {
                printf("Bad Input: Number too big\n");
                return -1;
            }

            if (!isdigit(argv[1][i])) {
                printf("Bad Input: Invalid number\n");
                return -1;
            }

            int digit = argv[1][i] - '0';
            number = (number * 10) + digit;
        }

        roman = toRoman(number + 1);
    } else {
        size_t len = mbstowcs(NULL, argv[1], 0);
        if (len == (size_t) -1) {
            perror("Bad Input: Could not convert to normal input form\n");
            return -1;
        }

        roman = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
        mbstowcs(roman, argv[1], len + 1);

        int number = toNumber(roman);
        if (number == -1) {
            free(roman);
            roman = NULL;
        } else {
            roman = toRoman(number + 1);
        }
    }

    if (roman != NULL) {
        printf("Next Number: %ls\n", roman);
        free(roman);
    }
    
    return 0;
}