#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "error_handler.h"
#include "../general/general.h"
#include "../compiler/compiler.h"


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

    compiler_destroy();
    exit(error_type);
}

const char* error_handler_error_to_str(Error_Type error_type)
{
    switch (error_type)
    {
        case Error_Lexical: return "Lexical Error";
        case Error_Syntax: return "Syntax Error";
        case Error_Semantic: return "Semantic Error";

        default: return "Error";
    }
}
