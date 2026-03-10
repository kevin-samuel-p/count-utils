#include "factors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <limits.h>


const wchar_t *MULTIPLICATION_SYMBOL =
    /* Standard multiplication */
    L"\u00D7"  /* × MULTIPLICATION SIGN */
    L"\u2715"  /* ✕ MULTIPLICATION X */
    L"\u2716"  /* ✖ HEAVY MULTIPLICATION X */

    /* N-ary / vector multiplication variants */
    L"\u2A09"  /* ⨉ N-ARY TIMES OPERATOR */
    L"\u2A2F"  /* ⨯ VECTOR OR CROSS PRODUCT */

    /* Dot-based multiplication */
    L"\u00B7"  /* · MIDDLE DOT */
    L"\u22C5"  /* ⋅ DOT OPERATOR */
    L"\u2219"  /* ∙ BULLET OPERATOR */

    /* Bullet substitutions people misuse */
    L"\u2022"  /* • BULLET */
    L"\u2027"  /* ‧ HYPHENATION POINT */

    /* Asterisk variants */
    L"\u002A"  /* * ASTERISK */
    L"\u2217"  /* ∗ ASTERISK OPERATOR */
    L"\u2731"  /* ✱ HEAVY ASTERISK */
    L"\u2732"  /* ✲ OPEN CENTER ASTERISK */
    L"\u2733"  /* ✳ EIGHT SPOKED ASTERISK */

    /* ASCII fallbacks */
    L"xX"
;

const wchar_t SUPERSCRIPT[] =
{
    L'\u2070',      // ⁰
    L'\u00b9',      // ¹
    L'\u00b2',      // ²
    L'\u00b3',      // ³
    L'\u2074',      // ⁴
    L'\u2075',      // ⁵
    L'\u2076',      // ⁶
    L'\u2077',      // ⁷
    L'\u2078',      // ⁸
    L'\u2079',      // ⁹
    L'\0'
};

void intToSuperscript(int exponent, wchar_t *buffer)
{
    if (exponent == 1)
    {
        buffer[0] = L'\0';
        return;
    }

    int digCount =  1;
    int multi = 10;

    while (multi <= exponent)
    {
        multi *= 10;
        ++digCount;
    }

    for (int i = digCount - 1; i >= 0; i--)
    {
        int digit = exponent % 10;
        buffer[i] = SUPERSCRIPT[digit];
        exponent /= 10;
    }

    buffer[digCount] = L'\0';
}

int superscriptToInt(wchar_t **superscript)
{
    if (!superscript || !*superscript) return 0;

    int n = wcslen(*superscript);
    if (n == 0) return 0;

    int exponent = -1;

    for (int i = 0; i < n; i++)
    {
        wchar_t *digit = wcschr(SUPERSCRIPT, **superscript);
        if (!digit)
            return exponent;    // Return early

        if (exponent < 0) ++exponent;

        exponent = (exponent * 10) + (digit - SUPERSCRIPT);
        ++*superscript;
    }

    return exponent;
}

wchar_t *factorize(unsigned long long number)
{
    if (number == 0)
    {
        printf("Bad Input - Zero cannot be factorized.\n");
        return NULL;
    }

    wchar_t *expression;
    unsigned long long factors[16] = {0};   // Prime factors
    int powers[16] = {0};                   // Corresponding powers
    int idx = 0;

    if (number == 1)
    {
        expression = malloc(2 * sizeof(wchar_t));
        if (!expression)
        {
            printf("Error - malloc failure.\n");
            return NULL;
        }

        expression[0] = L'1';
        expression[1] = L'\0';

        return expression;
    }

    unsigned long long copy = number;

    // Check divisibility by 2
    while ((copy & 1) == 0)
    {
        copy >>= 1;     // Divide by two
        
        if (factors[idx] == 0)
            factors[idx] = 2;   // Set factor value

        ++powers[idx];      // Increase power of two
    }

    if (powers[idx] != 0)
        ++idx;      // Increment to find next factor

    // Check divisibility by 3
    while (copy % 3 == 0)
    {
        copy /= 3;      // Divide by three

        if (factors[idx] == 0)
            factors[idx] = 3;   // Set factor value

        ++powers[idx];      // Increase power of three
    }

    if (powers[idx] != 0)
        ++idx;      // Increment to find next factor

    // Check divisibility by all other odd prime numbers
    for (unsigned long long k = 5ULL; k <= copy / k; k += 6) 
    {
        for (int p = k; p <= k + 2; p += 2)
        {
            while (copy % p == 0)
            {
                copy /= p;

                if (factors[idx] == 0)
                    factors[idx] = p;

                ++powers[idx];
            }

            if (powers[idx] != 0)
                ++idx;
        }
    }

    if (copy > 1)
    {
        factors[idx] = copy;
        powers[idx] = 1;
        ++idx;
    }

    size_t capacity = 128;
    expression = malloc(capacity * sizeof(wchar_t));
    if (!expression)
    {
        printf("Error - malloc failure.\n");
        return NULL;
    }

    int n = 0, written;
    unsigned long long factor;
    wchar_t power[10] = {0};

    for (int i = 0; i < idx; i++)
    {
        factor = factors[i];
        intToSuperscript(powers[i], power);

        written = ((i > 0) ? 
            swprintf(
                expression + n,
                capacity - n,
                L" %lc ",
                *MULTIPLICATION_SYMBOL
            ) : 0
        );

        if (written < 0)
        {
            printf("Error - Internal conversion failure.\n");
            free(expression);
            return NULL;
        }

        n += written;

        written = swprintf(
            expression + n, 
            capacity - n, 
            L"%llu%ls", 
            factor, power
        );

        if (written < 0)
        {
            printf("Error - Internal conversion failure.\n");
            free(expression);
            return NULL;
        }

        n += written;
    }
    expression[n] = L'\0';

    // Attempt realloc
    wchar_t *tmp = realloc(expression, (n + 1) * sizeof(wchar_t));
    if (tmp)
        expression = tmp;

    return expression;
}

unsigned long long multiply(const wchar_t *expression)
{
    if (!expression)
    {
        printf("Bad Input - NULL string.\n");
        return 0;
    }

    size_t len = wcslen(expression);
    if (len <= 0)
    {
        printf("Bad Input - Malformed expression.\n");
        return 0;
    }

    int power;
    unsigned long long 
        factor = 0,
        number = 1
    ;
    
    enum Flag
    {
        FACTOR,
        POWER,
        MULTIPLIER,
        SPACE
    } 
    lastParsed = MULTIPLIER;

    for (wchar_t *ptr = expression; *ptr != L'\0'; ptr++)
    {
        if (*ptr >= L'0' && *ptr <= L'9')
        {
            if (lastParsed == POWER || lastParsed == SPACE)
            {
                printf("Bad Input - Malformed expression.\n");
                return 0;
            }

            int digit = *ptr - L'0';

            if (factor > ULLONG_MAX / 10)
            {
                printf("Bad Input - Expression too large.\n");
                return 0;
            }
            factor *= 10;

            if (factor > ULLONG_MAX - digit)
            {
                printf("Bad Input - Expression too large.\n");
                return 0;
            }
            factor += digit;

            lastParsed = FACTOR;
        }
        else if (iswspace(*ptr))
        {
            if (lastParsed == MULTIPLIER) 
                continue;
            lastParsed = SPACE;
        }
        else if (wcschr(MULTIPLICATION_SYMBOL, *ptr))
        {
            if (lastParsed == MULTIPLIER)
            {
                printf("Bad Input - Malformed expression.\n");
                return 0;
            }

            if (
                lastParsed == FACTOR || 
                lastParsed == SPACE && factor != 0
            ) {
                if (number >= ULLONG_MAX / factor)
                {
                    printf("Bad Input - Expression is too large.\n");
                    return 0;
                }
                number *= factor;
                factor = 0;     // Reset factor (IMPORTANT)
            }
            lastParsed = MULTIPLIER;
        }
        else
        {
            if (lastParsed == MULTIPLIER || lastParsed == SPACE)
            {
                printf("Bad Input - Malformed expression.\n");
                return 0;
            }
            
            power = superscriptToInt(&ptr);
            if (power == -1)
            {
                printf("Bad Input - Malformed expression.\n");
                return 0;
            }

            for (int i = 0; i < power; i++)
            {
                if (number >= ULLONG_MAX / factor)
                {
                    printf("Bad Input - Expression is too large.\n");
                    return 0;
                }
                number *= factor;
            }

            factor = 0;     // Reset factor (IMPORTANT)
            --ptr;          // Reset pointer (IMPORTANT)

            lastParsed = POWER;
        }
    }

    if (lastParsed == MULTIPLIER)
    {
        printf("Bad Input - Malformed expression.\n");
        return 0;
    }

    if (factor != 0)
    {
        if (number >= ULLONG_MAX / factor)
        {
            printf("Bad Input - Expression is too large.\n");
            return 0;
        }
        number *= factor;
    }

    return number;
}