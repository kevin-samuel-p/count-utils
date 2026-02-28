#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "platform.h"

#include "emoji.h"
#include "increasing.h"
#include "meme.h"
#include "morse.h"
#include "nihongo.h"
#include "nwnception.h"
#include "radix.h"
#include "reps.h"
#include "reversals.h"
#include "roman.h"
#include "tally.h"

#include "ModeSelector.h"
#include "MiscUtils.h"


// -------- HELPER FUNCTIONS --------

void conversion_output(const char *string)
{
    printf("\n%s\n", string);
    if (copy_utf8_to_clipboard(string))
        printf("Copied value to clipboard!\n");
}

void conversion_output_w(const wchar_t *string)
{
    print_wide(string);
    if (copy_to_clipboard(string))
        printf("Copied value to clipboard!\n");
}


// -------- ACTION FUNCTIONS --------

bool convert_base(const char *number, enum CountingMode mode, enum Radix convertTo)
{
    enum Radix convertFrom;

    switch(mode)
    {
        case MODE_BINARY:
            convertFrom = BINARY;
        break;

        case MODE_DECIMAL:
            convertFrom = DECIMAL;
        break;

        case MODE_HEXADECIMAL:
            convertFrom = HEXADECIMAL;
        break;

        case MODE_OCTAL:
            convertFrom = OCTAL;
        break;
    }

    char *converted = convert(number, convertFrom, convertTo);
    if (!converted) 
        return false;

    conversion_output(converted);
    free(converted);

    return true;
}

bool convert_emoji(const char *number)
{
    char *converted = number_to_emoji(number);
    if (!converted)
        return false;

    conversion_output(converted);
    free(converted);

    return true;
}

void check_increasing(const char *number)
{
    // Use only with verified, sanitized input
    if (has_increasing_digits(number))
        printf("Yes! %s has increasing digits!\n", number);
    else
        printf("No, %s does not have increasing digits.\n", number);
}

bool convert_japanese(const char *string, bool isNumber)
{
    if (isNumber)
    {
        wchar_t *converted = translate_to_japanese(string);
        if (!converted)
            return false;

        conversion_output_w(converted);
        free(converted);
    }
    else
    {
        wchar_t *transformedInput = utf8_to_wide(string);
        if (!transformedInput)
            return false;
            
        char *converted = translate_from_japanese(transformedInput);
        free(transformedInput);
        
        if (!converted)
            return false;

        conversion_output(converted);
        free(converted);
    }

    return true;
}

bool convert_morse(const char *string, bool isNumber)
{
    char *converted;

    if (isNumber)
    {
        converted = translate_to_morse_code(string);
    }
    else
    {
        converted = translate_from_morse_code(string);
    }

    if (!converted)
        return false;

    conversion_output(converted);
    free(converted);

    return true;
}

void check_norep(unsigned long long number)
{
    if (is_repeating(number))
        printf("No, %llu is a repeating number.\n", number);
    else
        printf("Yes! %llu is not a repeating number.\n", number);
}

void check_palindrome(const char *string)
{
    // Use only verified, sanitized input
    if (is_palindrome(string))
        printf("Yes! %s is a palindrome.\n", string);
    else
        printf("No, %s is not a palindrome.\n", string);
}

void check_rep(unsigned long long number)
{
    if (is_repeating(number))
        printf("Yes! %llu is a repeating number.\n", number);
    else
        printf("No, %llu is not a repeating number.\n", number);
}

bool convert_roman(const char *string, bool isNumber, char representation)
{
    if (isNumber)
    {
        wchar_t *converted = number_to_roman(string, representation);
        if (!converted)
            return false;

        conversion_output_w(converted);
        free(converted);
    }
    else
    {
        wchar_t *transformedInput = utf8_to_wide(string);
        if (!transformedInput)
            return false;

        char *converted = roman_to_number(transformedInput);
        free(transformedInput);

        if (!converted)
            return false;

        conversion_output(converted);
        free(converted);
    }

    return true;
}

bool convert_tally(const char *string, bool isNumber)
{
    char *converted;

    if (isNumber)
    {
        unsigned long long *num = NULL;
        string_to_number(string, &num);
        if (!num || *num > INT_MAX)
            return false;

        converted = tally(*(int *)num, DEFAULT);
        free(num);

        if (!converted)
            return false;
    }
    else
    {
        int num = parse_tally_marks(string);
        if (num == -1)
            return false;

        converted = malloc(16);
        if (!converted)
        {
            printf("Error - malloc failure.\n");
            return false;
        }

        number_to_string(converted, num, DECIMAL);
    }

    conversion_output(converted);
    free(converted);

    return true;
}