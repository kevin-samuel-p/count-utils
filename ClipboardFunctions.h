#include <stdio.h>
#include <windows.h>


int copy_to_clipboard(const wchar_t *);
int copy_utf8_to_clipboard(const char *);


int copy_to_clipboard(const wchar_t *text) 
{
    if (!OpenClipboard(NULL)) return 0;
        
    EmptyClipboard();

    size_t size = (wcslen(text) + 1) * sizeof(wchar_t);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hMem) 
    {
        CloseClipboard();
        return 0;
    }

    void *mem = GlobalLock(hMem);
    memcpy(mem, text, size);
    GlobalUnlock(hMem);

    SetClipboardData(CF_UNICODETEXT, hMem);

    CloseClipboard();
    return 1;
}


// ANSI Wrapper for UTF-8/ASCII
int copy_utf8_to_clipboard(const char *text) 
{
    int wcharCount = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    if (wcharCount == 0) 
        return 0;

    wchar_t *wtext = (wchar_t *)malloc(wcharCount * sizeof(wchar_t));
    if (!wtext) 
        return 0;

    MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, wcharCount);

    int result = copy_to_clipboard(wtext);
    free(wtext);

    return result;
}
