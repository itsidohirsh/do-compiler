#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "error_handler.h"


void printf_red(const char* format, ...)
{
    printf("\033[0;31m");   // Make printing red

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

const char* error_handler_error_to_str(Error_Type error_type)
{
    switch (error_type)
    {
        case Error_Lexer: return "Lexical Error";
        case Error_Parser: return "Syntax Error";
        case Error_Memory: return "Memory allocation Error";
        case Error_File_IO: return "File I/O Error";

        default: return "Error";
    }
}

void error_handler_report(int line, Error_Type error_type, char* format, ...)
{
    printf("[");
    printf_red(error_handler_error_to_str(error_type));
    printf(" on line");
    printf_cyan(" %d", line);
    printf("] ");

    va_list args;           // Declare a va_list type variable
    va_start(args, format); // Initialize the va_list with the ...
    vprintf(format, args);  // Forward ... to vprintf
    va_end(args);           // Clean up the va_list

    printf("\n");
    exit(error_type);
}

void error_handler_report_memory_error()
{
    printf("[");
    printf_red(error_handler_error_to_str(Error_Memory));
    printf("] Failed to allocate needed memory\n");

    exit(Error_Memory);
}

void error_handler_report_file_IO_error(char* file_name)
{
    printf("[");
    printf_red(error_handler_error_to_str(Error_File_IO));
    printf("] Could not read file %s\n", file_name);

    exit(Error_File_IO);
}
