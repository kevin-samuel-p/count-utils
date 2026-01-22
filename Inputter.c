#include "Inputter.h"

#include <stdio.h>
#include <stdlib.h>


const char TEMP_FILE_NAME[] = "Enter the number here, press Ctrl + S, and the file..txt";


int create_temp_file() 
{
    FILE *fp = fopen(TEMP_FILE_NAME, "w");
    if (!fp) return 0;

    fclose(fp);
    return 1;
}


int await_user_input() 
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), 
             "notepad \"%s\"", 
             TEMP_FILE_NAME);
    return system(cmd) == 0;
}


int delete_temp_file()
{
    return remove(TEMP_FILE_NAME) == 0;
}