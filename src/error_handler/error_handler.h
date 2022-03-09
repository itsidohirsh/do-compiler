#pragma once


/* ---------- Structs ---------- */

// Enum for possible error types, also used for exit codes
typedef enum Error_Type
{
    Error_General = 1,
    Error_Lexer,
    Error_Parser,
    Error_Memory,
    Error_File_IO,
} Error_Type;


/* ---------- Functions ---------- */

// printf in different colors
void printf_red(const char* format, ...);
void printf_cyan(const char* format, ...);

// Converts and error code to it's string representation
const char* error_handler_error_to_str(Error_Type error_type);

// Reports an error message to the user and exits
void error_handler_report(int line, Error_Type error_type, char* format, ...);

// Reports an allocation error message to the user and exits
void error_handler_report_memory_error();

// Reports a file I/O error message to the user and exits
void error_handler_report_file_IO_error(const char* filename);
