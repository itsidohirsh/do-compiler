#pragma once


/* ---------- Structs ---------- */

// Enum for possible error types, also used for exit codes
typedef enum Error_Type
{
    Error_General = 1,
    Error_Lexical,
    Error_Syntax,
    Error_Semantic,
} Error_Type;


/* ---------- Functions ---------- */

// Reports an error message to the user and exits
void error_handler_report(int line, Error_Type error_type, char* format, ...);

// Converts and error code to it's string representation
const char* error_handler_error_to_str(Error_Type error_type);
