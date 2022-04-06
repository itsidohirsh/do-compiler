#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "ansi.h"

void ansi_setup_console()
{
    DWORD outMode = 0;
    HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (stdoutHandle == INVALID_HANDLE_VALUE)
        exit(GetLastError());

    if (!GetConsoleMode(stdoutHandle, &outMode))
        exit(GetLastError());

    // Enable ANSI escape codes for Windows console
    outMode |= 0x0004;

    if (!SetConsoleMode(stdoutHandle, outMode))
        exit(GetLastError());
}
