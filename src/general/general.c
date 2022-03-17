#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "general.h"
#include "../compiler/compiler.h"


void exit_memory_error(char* file, int line)
{
    printf("[");
    printf_red("Memory Error");
    printf("] Failed to allocate needed memory\n");
    printf("File: ");
    printf_purple("%s\n", file);
    printf("On line: ");
    printf_cyan("%d\n", line);

    compiler_destroy();
    exit(1);
}

void exit_file_io_error(char* file_name)
{
    printf("[");
    printf_red("File I/O Error");
    printf("] Could not read file ");
    printf_purple("%s\n", file_name);

    compiler_destroy();
    exit(1);
}

void printf_red(const char* format, ...)
{
    printf("\033[0;31m");   // Make printing red

    va_list args;           // Declare a va_list type variable
    va_start(args, format); // Initialize the va_list with the ...
    vprintf(format, args);  // Forward ... to vprintf
    va_end(args);           // Clean up the va_list

    printf("\033[0m");      // Reset printing color
}

void printf_purple(const char* format, ...)
{
    printf("\033[0;35m");   // Make printing purple

    va_list args;           // Declare a va_list type variable
    va_start(args, format); // Initialize the va_list with the ...
    vprintf(format, args);  // Forward ... to vprintf
    va_end(args);           // Clean up the va_list

    printf("\033[0m");      // Reset printing color
}

void printf_cyan(const char* format, ...)
{
    printf("\033[0;36m");   // Make printing cyan

    va_list args;           // Declare a va_list type variable
    va_start(args, format); // Initialize the va_list with the ...
    vprintf(format, args);  // Forward ... to vprintf
    va_end(args);           // Clean up the va_list

    printf("\033[0m");      // Reset printing color
}
