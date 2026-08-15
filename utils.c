#include "utils.h"

long get_filesize(FILE* f) {
    int fs;
    fseek(f, 0, SEEK_END);
    fs = ftell(f);
    rewind(f);
    return fs;
}

void keypress(BYTE key) {
    // TODO: find a way to send keystrokes to inactive window (window in background)
    // None of those methods work when window is inactive.
    // You HAVE to keep game window active in order keypresses to work.
    // Maybe code injection should do the job...
    /*
    PostMessage(pw_hwnd, WM_KEYDOWN, key, 0);
    Sleep(10);
    PostMessage(pw_hwnd, WM_KEYUP, key, 0);
    */
    SendMessage(pw_hwnd, WM_KEYDOWN, key, 0);
    Sleep(10);
    SendMessage(pw_hwnd, WM_KEYUP, key, 0);
}
