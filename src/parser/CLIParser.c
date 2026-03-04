#include <stdio.h>
#include <stdlib.h>

#include "ModeSelector.h"
#include "OptionSelector.h"

#include "CustomActions.h"
#include "HelpActions.h"
#include "Inputter.h"
#include "MiscUtils.h"
#include "ParamParser.h"
#include "RunActions.h"

int main(int argc, char *argv[])
{
    int exit_status = 1;

    enum CountMode mode;
    char option, param;
    char *input = NULL;
    unsigned long long *val = NULL;

    // If no args, print info
    if (argc == 1)
    {
        if (!read_docs(0))
            goto Done;
    }

    // Find mode
    mode = find_mode(argv[1]);

    // Help mode
    if (mode == MODE_HELP)
    {
        if (argc == 2)
        {
            if (!read_docs(MODE_HELP))
                goto Done;
        }
        else
        {
            enum CountMode helpTopic = find_mode(argv[2]);
            if (!~helpTopic || helpTopic == MODE_HELP)
            {
                printf(
                    "Invalid Syntax - Command `counter help %s` does not exist.\n"
                    "For supported commands, type\n"
                    "    counter help\n",
                    argv[2]
                );
                goto Done;
            }
        }

        goto Success;
    }

    // If not help mode, then counting mode
    // Find option
    option = is_run_option(argv[2]);

    if (!option)
        option = is_custom_option(argv[2], mode);

    if (!option)
        goto Done;

    // Check for parameter
    bool takes_param = param_satisfiability(argc, mode, option);
    if (takes_param)
    {
        if (argc < 4)
        {
            printf(
                "Invalid Syntax - Expected fourth parameter.\n"
                "    counter %s %s <param>\n"
                "For more information, type `counter help %s`\n",
                argv[1], argv[2], argv[1]
            );
            goto Done;
        }

        param = parse_radix_param(argv[3], mode);
        if (!param)
        {
            printf(
                "Invalid Syntax - Command `counter %s %s %s` does not exist.\n"
                "For supported commands, type\n"
                "    counter help %s\n",
                argv[1], argv[2], argv[3], argv[1]
            );
            goto Done;
        }
    }

    if ((takes_param) ? argc > 4 : argc > 3)
        printf("Warning - Extra arguments will be ignored...\n\n");

    // Create temporary input file
    if (!create_temp_file())
    {
        printf(
            "Error - Could not create input file.\n"
            "Please make sure you are running this tool as an administrator.\n"
        );
        goto Done;
    }

    if (!await_user_input())
    {
        printf(
            "Error - Could not open default file editor.\n"
            "Please check whether your computer has a default file editor.\n"
        );
        goto Done;
    }

    // Read user input
    input = read_temp_file_utf8();
    if (!input)
        goto Done;

    // Perform action
    switch(mode)
    {
        case MODE_BINARY:
        case MODE_DECIMAL:
        case MODE_HEXADECIMAL:
        case MODE_OCTAL:
            sanitize(&input);
            if (option == 'c' && !convert_base(input, mode, param)) goto Done;
            if (option != 'c' && !run_radix(input, option, mode)) goto Done;
        break;

        case MODE_EMOJI:
            if (!sanitize(&input)) goto Done;
            if (option == 'c' && !convert_emoji(input)) goto Done; 
            if (option != 'c' && !run_emoji(input, option)) goto Done;
        break;

        case MODE_INCREASING:
            if (!sanitize(&input)) goto Done;
            if (option == 'c') check_increasing(input);
            if (option != 'c' && !run_increasing(input, option)) goto Done;
        break;

        case MODE_JAPANESE:
            bool isNumber = sanitize(&input);
            if (option == 'c' && !convert_japanese(input, isNumber)) goto Done;
            if (option != 'c' && !run_japanese(input, option, isNumber)) goto Done;
        break;

        case MODE_MIRROR:
            if (param == 'n' && !sanitize(&input)) goto Done;
            if (!run_mirror(input, option, param)) goto Done;
        break;

        case MODE_MORSE:
            bool isNumber = sanitize(&input);
            if (option == 'c' && !convert_morse(input, isNumber)) goto Done;
            if (option != 'c' && !run_morse(input, option, isNumber)) goto Done;
        break;

        case MODE_NOREP:
            if (!sanitize(&input)) goto Done;
            string_to_number(input, &val);
            if (!val) goto Done;

            if (option == 'c') check_norep(*val);
            if (option != 'c' && !run_norep(*val, option)) goto Done;
        break;

        case MODE_NWN:
        case MODE_NWNWN:
        case MODE_NWNWNN:
            if (!run_nwns(input, option, mode)) goto Done;
        break;

        case MODE_PALINDROME:
            if (!sanitize(&input)) goto Done;
            if (option == 'c') check_palindrome(input);
            if (option != 'c' && !run_palindrome(input, option)) goto Done;
        break;

        case MODE_REP:
            if (!sanitize(&input)) goto Done;
            string_to_number(input, &val);
            if (!val) goto Done;

            if (option == 'c') check_rep(*val);
            if (option != 'c' && !run_rep(*val, option)) goto Done;
        break;

        case MODE_ROMAN:
            bool isNumber = sanitize(&input);
            if (option == 'c' && !convert_roman(input, isNumber, param)) goto Done;
            if (option != 'c' && !run_roman(input, option, param, isNumber)) goto Done;
        break;

        case MODE_TALLY:
            bool isNumber = sanitize(&input);
            if (option == 'c' && !convert_tally(input, isNumber)) goto Done;
            if (option != 'c' && !run_tally(input, option, param, isNumber)) goto Done;
        break;
    }

Success:
    --exit_status;

Done:
    delete_temp_file();
    free(input);
    free(val);

    return exit_status;
}