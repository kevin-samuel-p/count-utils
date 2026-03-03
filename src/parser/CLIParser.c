#include <stdio.h>
#include <stdlib.h>

#include "platform.h"

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
            if (helpTopic == -1 || helpTopic == MODE_HELP)
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
    if (option == 'c' && (
            mode == MODE_BINARY || 
            mode == MODE_DECIMAL || 
            mode == MODE_HEXADECIMAL || 
            mode == MODE_OCTAL
        )
    ) {
        if (argc < 4)
        {
            printf(
                "Invalid Syntax - Expected fourth parameter.\n"
                "    counter %s %s <target>\n"
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
    
    if (mode == MODE_MEME)
    {
        if (argc < 4)
        {
            printf(
                "Invalid Syntax - Expected fourth parameter.\n"
                "    counter %s %s <meme>\n"
                "For more information, type `counter help %s`\n",
                argv[1], argv[2], argv[1]
            );
            goto Done;
        }

        param = parse_meme_param(argv[3]);
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
    
    if (mode == MODE_MIRROR)
    {
        if (argc < 4)
        {
            printf(
                "Invalid Syntax - Expected fourth parameter.\n"
                "    counter %s %s <input_mode>\n"
                "For more information, type `counter help %s\n",
                argv[1], argv[2], argv[1]
            );
            goto Done;
        }

        param = parse_mirror_param(argv[3]);
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
    
    if (mode == MODE_ROMAN)
    {
        if (argc < 4)
        {
            printf(
                "Invalid Syntax - Expected fourth parameter.\n"
                "    counter %s %s <representation>\n"
                "For more information, type `counter help %s\n",
                argv[1], argv[2], argv[1]
            );
            goto Done;
        }

        param = parse_roman_param(argv[3]);
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
    
    if (mode == MODE_TALLY)
    {
        if (argc < 4)
        {
            printf(
                "Invalid Syntax - Expected fourth parameter.\n"
                "    counter %s %s <formatting>\n"
                "For more information, type `counter help %s\n",
                argv[1], argv[2], argv[1]
            );
            goto Done;
        }

        param = parse_tally_param(argv[3]);
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
    {}

Success:
    --exit_status;

Done:
    delete_temp_file();
    free(input);

    return exit_status;
}