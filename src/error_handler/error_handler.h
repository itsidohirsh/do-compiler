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

// A function that helps the compiler to recover after encountering a syntax error.
// "eats" tokens from the input until finds a token that is at the start of a statment production rule.
// When finds one, pops every token that is no longer relevant (shifted in the current errorful statement) off the stack,
// and shifts said found token with it's correct starting state onto the updated stack.
// At the end, retrieves the next token from the input.
void error_handler_error_recovery();

// Prints an expected prog error message to the user, and pushes a prog entry into the parse stack
void error_handler_report_expected_prog();

// Prints an expected prog id error message to the user, and pushes an id entry into the parse stack
void error_handler_report_expected_prog_id();

// Prints an expected colon error message to the user, and pushes a `:` entry onto the parse stack
// with the right goto state according to the state in the action table
void error_handler_report_expected_colon_state_3();
void error_handler_report_expected_colon_state_37();
void error_handler_report_expected_colon_state_46();
void error_handler_report_expected_colon_state_59();

// Prints an expected `;` error message to the user
void error_handler_report_expected_semi_colon();

// Prints an expected identifier error message to the user
void error_handler_report_expected_identifier();

// Prints an expected `(` error message to the user
void error_handler_report_expected_open_paren();

// Prints an expected `=` error message to the user
void error_handler_report_expected_assign();

// Converts and error code to it's string representation
const char* error_handler_error_to_str(Error_Type error_type);
