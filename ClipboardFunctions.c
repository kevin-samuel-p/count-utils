#include "ClipboardFunctions.h"

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>


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

    SetClipboardData(CF_UNICODETEXT, hMem);

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
