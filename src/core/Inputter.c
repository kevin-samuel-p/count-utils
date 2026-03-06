#include "Inputter.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "platform.h"

const char TEMP_FILE_NAME[] = 
    "Enter the number here, "
    "save, and close the file..txt"
;

char path[FILENAME_MAX] = {0};

bool create_temp_file() 
{
    if (!get_temp_file_path(path, sizeof(path), TEMP_FILE_NAME))
        return false;
    
    FILE *fp = fopen(path, "w");
    if (!fp) return false;

    fclose(fp);
    return true;
}

bool await_user_input() 
{
    char cmd[256];

    // Check for $EDITOR environment variable (Linux/macOS)
    const char *editor = getenv("EDITOR");

    if (!editor)
        snprintf(
            cmd, sizeof(cmd),
            EDITOR_CMD,
            path
        );
    else
        snprintf(
            cmd, sizeof(cmd),
            "%s \"%s\"",
            editor, path
        );

    return system(cmd) == 0;
}

char *read_temp_file_utf8()
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
    {
        printf("Error - Could not open input file.\n");
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END) != 0)
    {
        printf("Error - Failed to determine size of input file.\n");
        fclose(fp);
        return NULL;
    }

    long size = ftell(fp);
    if (size < 0)
    {
        printf("Error - Failed to determine size of input file.\n");
        fclose(fp);
        return NULL;
    }

    rewind(fp);

    char *buf = malloc((size_t)size + 1);
    if (!buf)
    {
        printf("Error - Out of memory while reading input file.\n");
        fclose(fp);
        return NULL;
    }

    size_t read = fread(buf, 1, (size_t)size, fp);
    fclose(fp);

    if (read != (size_t)size)
    {
        printf("Error - Failed to read input file completely.\n");
        free(buf);
        return NULL;
    }

    buf[size] = '\0';

    /* ---- UTF-8 BOM detection ---- */
    if (size >= 3)
    {
        unsigned char b0 = (unsigned char)buf[0];
        unsigned char b1 = (unsigned char)buf[1];
        unsigned char b2 = (unsigned char)buf[2];

        if (b0 == 0xEF && b1 == 0xBB && b2 == 0xBF)
        {
            // Skip BOM
            memmove(buf, buf + 3, size - 3);
            size -= 3;
            buf[size] = '\0';
        }
    }

    /* ---- UTF-16 BOM detection ---- */
    if (size >= 2)
    {
        unsigned char b0 = (unsigned char)buf[0];
        unsigned char b1 = (unsigned char)buf[1];

        if (
            b0 == 0xFF && b1 == 0xFE ||
            b0 == 0xFE && b1 == 0xFF
        ) {
            printf(
                "Bad Input - File is encoded in UTF-16.\n"
                "Please save the file as UTF-8 and try again.\n"
            );
            free(buf);
            return NULL;
        }
    }

    return buf;
}

bool delete_temp_file()
{
    return remove(path) == 0;
}