#if defined(__linux__) || defined(__APPLE__)

#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include <termios.h>
#include <unistd.h>


wchar_t *utf8_to_wide(const char *utf8)
{
    if (!utf8)
    {
        printf("Bad Input - NULL string.\n");
        return NULL;
    }

    /* Ensure locale is UTF-8 aware */
    setlocale(LC_ALL, "");

    size_t len = mbstowcs(NULL, utf8, 0);
    if (len == (size_t)-1)
    {
        printf("Bad Input - Input contains malformed UTF-8 sequences.\n");
        return NULL;
    }

    wchar_t *wide = malloc((len + 1) * sizeof(wchar_t));
    if (!wide)
    {
        printf("Error - malloc failure.\n");
        return NULL;
    }

    if (mbstowcs(wide, utf8, len + 1) == (size_t)-1)
    {
        printf("Error - Could not process multibyte string.\n");
        free(wide);
        return NULL;
    }

    return wide;
}

void print_wide(const wchar_t *text)
{
    if (!text)
    {
        printf("Bad Input - NULL string.\n");
        return;
    }

    setlocale(LC_ALL, "");
    wprintf(L"%ls\n", text);
}

static bool run_command_with_wide_input(const char *command, const wchar_t *text)
{
    if (!text)
        return false;

    setlocale(LC_ALL, "");

    /* Convert wide → UTF-8 */
    size_t len = wcstombs(NULL, text, 0);
    if (len == (size_t)-1)
        return false;

    char *utf8 = malloc(len + 1);
    if (!utf8)
        return false;

    wcstombs(utf8, text, len + 1);

    FILE *pipe = popen(command, "w");
    if (!pipe)
    {
        free(utf8);
        return false;
    }

    fputs(utf8, pipe);
    int result = pclose(pipe);

    free(utf8);
    return result == 0;
}

bool copy_to_clipboard(const wchar_t *text)
{
#if defined(__APPLE__)
    return run_command_with_wide_input("pbcopy", text);
#elif defined(__linux__)
    /* Try Wayland first */
    if (run_command_with_wide_input("wl-copy", text))
        return true;

    /* Fallback to X11 */
    return run_command_with_wide_input("xclip -selection clipboard", text);
#else
    return false;
#endif
}

bool copy_utf8_to_clipboard(const char *text)
{
    if (!text)
        return false;

#if defined(__APPLE__)
    FILE *pipe = popen("pbcopy", "w");
#elif defined(__linux__)
    FILE *pipe = popen("wl-copy 2>/dev/null", "w");
    if (!pipe)
        pipe = popen("xclip -selection clipboard", "w");
#else
    FILE *pipe = NULL;
#endif

    if (!pipe)
        return false;

    fputs(text, pipe);
    int result = pclose(pipe);

    return result == 0;
}

wchar_t *read_clipboard()
{
    FILE *pipe = NULL;

#if defined(__APPLE__)
    pipe = popen("pbpaste", "r");
#elif defined(__linux__)
    pipe = popen("wl-paste 2>/dev/null", "r");
    if (!pipe)
        pipe = popen("xclip -selection clipboard -o", "r");
#else
    return NULL;
#endif

    if (!pipe)
        return NULL;

    char buffer[4096];
    size_t total = 0;
    char *utf8_result = NULL;

    while (fgets(buffer, sizeof(buffer), pipe))
    {
        size_t len = strlen(buffer);
        char *tmp = realloc(utf8_result, total + len + 1);
        if (!tmp)
        {
            free(utf8_result);
            pclose(pipe);
            return NULL;
        }

        utf8_result = tmp;
        memcpy(utf8_result + total, buffer, len);
        total += len;
        utf8_result[total] = '\0';
    }

    pclose(pipe);

    if (!utf8_result)
        return NULL;

    /* Convert UTF-8 → wide */
    wchar_t *wide = utf8_to_wide(utf8_result);
    free(utf8_result);

    return wide;
}


static struct termios original_termios;

void enable_raw_mode()
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &original_termios);
    raw = original_termios;

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void restore_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

int read_key()
{
    unsigned char c;

    if (read(STDIN_FILENO, &c, 1) <= 0)
        return KEY_OTHER;

    if (c == 27)    // ESC
        return KEY_ESC;

    if (c == '\t')  // TAB
        return KEY_TAB;

    return KEY_OTHER;
}

#endif