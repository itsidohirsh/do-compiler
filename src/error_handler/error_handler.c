#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "error_handler.h"


void printf_red(const char* format, ...)
{
    printf("\033[0;31m"); // Make printing red

    va_list args;           // Declare a va_list type variable
    va_start(args, format); // Initialize the va_list with the ...
    vprintf(format, args);  // Forward ... to vprintf
    va_end(args);           // Clean up the va_list

    printf("\033[0m"); // Reset printing color
}

void printf_cyan(const char* format, ...)
{
    printf("\033[0;36m"); // Make printing red

    va_list args;           // Declare a va_list type variable
    va_start(args, format); // Initialize the va_list with the ...
    vprintf(format, args);  // Forward ... to vprintf
    va_end(args);           // Clean up the va_list

    printf("\033[0m"); // Reset printing color
}

void error_handler_report(int line, char* format, ...)
{
    printf("[");
    printf_red("Error: ");
    printf("on line");
    printf_cyan(" %d", line);
    printf("] ");

    va_list args;           // Declare a va_list type variable
    va_start(args, format); // Initialize the va_list with the ...
    vprintf(format, args);  // Forward ... to vprintf
    va_end(args);           // Clean up the va_list

    printf("\n");
    exit(1);
}

void error_handler_report_memory_error()
{
    printf("[");
    printf_red("Allocation Error");
    printf("] Failed to allocate needed memory\n");

    exit(1);
}

void error_handler_report_file_error(const char* filename)
{
    printf("[");
    printf_red("I/O Error");
    printf("] Could not read file %s\n", filename);

    exit(1);
}
