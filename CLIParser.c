#include <stdio.h>
#include <string.h>

#include "ClipboardFunctions.h"
#include "Inputter.h"


/**
 *      ____________Command__Line__Syntax____________
 *      (argv[0]) <mode> <option> [option-parameters]
 * 
 *      <> - necessary arguments
 *      [] - optional parameters
 * 
 * 
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |      MODE      |    OPTION     |        PARAMS          |                        DESCRIPTION                        |
 *      |----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with binary numbers.      |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |   d / dec / decimal    | Converts an inputted binary number to decimal.            |
 *      |     binary     |               |------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |  h / hex / hexadecimal | Converts an inputted binary number to hexadecimal.        |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |     o / oct / octal    | Converts an inputted binary number to octal.              |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with decimal numbers.     |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |    b / bin / binary    | Converts an inputted decimal number to octal.             |
 *      | decimal/normal |               |------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |  h / hex / hexadecimal | Converts an inputted decimal number to hexadecimal.       |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |     o / oct / octal    | Converts an inputted decimal number to octal.             |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with emojified numbers.   |
 *      |      emoji     |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |          NIL           | Converts an inputted number into an emojified number.     |
 *      |                |  emojify / e  |                        |                                                           |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with hexadecimal numbers. |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |    b / bin / binary    | Converts an inputted hexadecimal number to binary.        |
 *      |   hexadecimal  |               |------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |    d / dec / decimal   | Converts an inputted hexadecimal number to decimal.       |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |     o / oct / octal    | Converts an inputted hexadecimal number to octal.         |   
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using numbers with digits in     |
 *      |                |               |                        | non-decreasing order.                                     |
 *      |   increasing   |---------------+------------------------+-----------------------------------------------------------+
 *      |                |   check / c   |          NIL           | Checks whether an inputted number's digits are in         |
 *      |                |               |                        | non-decreasing order.                                     |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using Japanese numbers.          |
 *      |    japanese    |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |          NIL           | Translates an inputted number to Japanese, and converts   |
 *      |                | translate / t |                        | an inputted Japanese number to its decimal form.          |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with octal numbers.       |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |    b / bin / binary    | Converts an inputted octal number to binary.              |
 *      |      octal     |               |------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |    d / dec / decimal   | Converts an inputted octal number to decimal.             |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |  h / hex / hexadecimal | Converts an inputted octal number to hexadecimal.         |   
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |          69            | Initiates a counting run using '69' numbers.              |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |      meme      |    run / r    |          420           | Initiates a counting run using '420' numbers.             |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |         69420          | Initiates a counting run using both '69' numbers and '420'|
 *      |                |               |                        | numbers.                                                  |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |       m / mirror       | Initiates a counting run using mirrored numbers, taking   |
 *      |                |               |                        | the initial number as mirrored input.                     |
 *      |     mirror     |    run / r    |------------------------+-----------------------------------------------------------+
 *      |                |               |       n / normal       | Initiates a counting run using mirrored numbers, taking   |
 *      |                |               |                        | the initial number as normal input.                       |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run in Morse code.                   |
 *      |      morse     |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |          NIL           | Converts an inputted Morse code to its numerical value,   |
 *      |                |               |                        | and an inputted number to Morse code.                     |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using numbers that have no       |
 *      |                |               |                        | recurring digits.                                         |
 *      |      norep     |---------------+------------------------+-----------------------------------------------------------+
 *      |                |   check / c   |          NIL           | Checks whether an inputted number contains no recurring   |
 *      |                |               |                        | digits.                                                   |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |       nwn      |    run / r    |          NIL           | Initiates a counting run using Numbers within Numbers.    |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |      nwnwn     |    run / r    |          NIL           | Initiates a counting run using Numbers within Numbers     |
 *      |                |               |                        | within Numbers.                                           |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |     nwnwnn     |    run / r    |          NIL           | Initiates a counting run using Numbers within Numbers     |
 *      |                |               |                        | within N Numbers.                                         |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using palindromic numbers.       |
 *      |   palindrome   |---------------+------------------------+-----------------------------------------------------------+
 *      |                |   check / c   |          NIL           | Checks whether an inputted number is a palindrome.        |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using numbers that have at least |
 *      |                |               |                        | one repeating digit.                                      |
 *      |       rep      |---------------+------------------------+-----------------------------------------------------------+
 *      |                |   check / c   |          NIL           | Checks whether an inputted number contains at least one   |
 *      |                |               |                        | repeating digit.                                          |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |                        | Initiates a counting run with Roman Numerals using their  |
 *      |                |               |     i / idiomatic      | idiomatic representations (inclusion of M as a recurring  |
 *      |                |               |                        | numeral in each chunk).                                   |
 *      |                |    run / r    |------------------------+-----------------------------------------------------------+
 *      |                |               |                        | Initiates a counting run with Roman Numerals using their  |
 *      |      roman     |               |        l / legal       | legal semantically correct standard representations       |
 *      |                |               |                        | (clustering groups of thousand by a single combining      |
 *      |                |               |                        | overline).                                                |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |          NIL           | Converts an inputted Roman number to its numerical value, |
 *      |                |               |                        | and an inputted number to its Roman numeral equivalent.   |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using tally marks.               |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |      tally     |               |                        | Converts an inputted number to its tally mark form        |
 *      |                |  convert / c  |          NIL           | (incorporating tally mark strikethrough according to      |
 *      |                |               |                        | Discord markdown rules), and counts tally marks to assess |
 *      |                |               |                        | its true numerical value.                                 |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 */


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Invalid Syntax: %s <mode> <option> [option-parameters]\n\nPlease review documentation for more information on the command line syntax...");
        return 1;
    }

    char option;
    char param[4] = {0};

    create_temp_file();

    if (strcmp(argv[1], "binary") == 0)
    {
        await_user_input();
        // Read file input
        // Call CLI as a process
    }
}