#ifdef _WIN32

#include <windows.h>
#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

static HANDLE hIn;
static DWORD original_mode;

wchar_t *utf8_to_wide(const char *utf8)
{
    if (!utf8)
    {
        printf("Bad Input - NULL string.\n");
        return NULL;
    }

    int len = MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        utf8,
        -1,
        NULL, 
        0
    );

    if (len == 0)
    {
        printf("Error - Could not process multibyte string.\n");
        return NULL;
    }

    wchar_t *wide = malloc((size_t)len * sizeof(wchar_t));
    if (!wide)
    {
        printf("Error - malloc failure.\n");
        return NULL;
    }

    if (!MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        utf8,
        -1,
        wide,
        len
    )) {
        printf("Bad Input - Input contains malformed UTF-8 sequences.\n");
        free(wide);
        return NULL;
    }

    return wide;
}

void print_wide(const wchar_t *text)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD written;
        WriteConsoleW(hOut, text, (DWORD)wcslen(text), &written, NULL);
        WriteConsoleW(hOut, L"\n", 1, &written, NULL);
    }
    else
    {
        wprintf(L"%ls\n", text); // fallback
    }
}

bool copy_to_clipboard(const wchar_t *text) 
{
    if (!OpenClipboard(NULL)) return false;
        
    EmptyClipboard();

    size_t size = (wcslen(text) + 1) * sizeof(wchar_t);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hMem) 
    {
        CloseClipboard();
        return false;
    }

    void *mem = GlobalLock(hMem);
    memcpy(mem, text, size);
    GlobalUnlock(hMem);

    if (!SetClipboardData(CF_UNICODETEXT, hMem)) {
        GlobalFree(hMem);
        CloseClipboard();
        return false;
    }

    CloseClipboard();
    return true;
}

bool copy_utf8_to_clipboard(const char *text) 
{
    int wcharCount = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    if (wcharCount == 0) 
        return false;

    wchar_t *wtext = malloc(wcharCount * sizeof(wchar_t));
    if (!wtext) 
        return false;

    MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, wcharCount);

    bool result = copy_to_clipboard(wtext);
    free(wtext);

    return result;
}

wchar_t *read_clipboard()
{
    wchar_t *result = NULL;

    if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
        return NULL;

    if (!OpenClipboard(NULL))
        return NULL;

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (!hData)
        goto cleanup;

    wchar_t *wtext = (wchar_t *)GlobalLock(hData);
    if (!wtext)
        goto cleanup;

    size_t len = wcslen(wtext) + 1;

    result = malloc(len * sizeof(wchar_t));
    if (result)
        memcpy(result, wtext, len * sizeof(wchar_t));

    GlobalUnlock(hData);

cleanup:
    CloseClipboard();
    return result;
}

void enable_raw_mode()
{
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hIn, &original_mode);

    SetConsoleMode(hIn,
        original_mode &
        ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT)
    );
}

void restore_mode()
{
    SetConsoleMode(hIn, original_mode);
}

int read_key()
{
    INPUT_RECORD ir;
    DWORD eventsRead;

    while (true)
    {
        ReadConsoleInput(hIn, &ir, 1, &eventsRead);
        if (ir.EventType != KEY_EVENT)
            continue;

        KEY_EVENT_RECORD key = ir.Event.KeyEvent;
        if (!key.bKeyDown)
            continue;

        switch(key.wVirtualKeyCode)
        {
            case VK_ESCAPE:
                return KEY_ESC;

            case VK_TAB:
                DWORD mods = key.dwControlKeyState;
                if (((mods & (
                    LEFT_ALT_PRESSED |
                    RIGHT_ALT_PRESSED |
                    LEFT_CTRL_PRESSED |
                    RIGHT_CTRL_PRESSED |
                    SHIFT_PRESSED)) == 0)
                ) { return KEY_TAB; }

            default:
                return KEY_OTHER;
        }
    }
}

#endif