#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
    #define EDITOR_CMD "notepad \"%s\""
#elif defined(__APPLE__)
    #define EDITOR_CMD "open -t \"%s\""
#else
    #define EDITOR_CMD "xdg-open \"%s\""
#endif

enum
{
    KEY_OTHER = 0,
    KEY_TAB,
    KEY_ESC
};

wchar_t *utf8_to_wide(const char *utf8);

void print_wide(const wchar_t *text);

bool copy_to_clipboard(const wchar_t *text);

bool copy_utf8_to_clipboard(const char *text);

wchar_t *read_clipboard(void);

void enable_raw_mode(void);

void restore_mode(void);

int read_key(void);

#endif /* PLATFORM_H */