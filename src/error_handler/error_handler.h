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

// Reports an error message to the user and updated the number of errors in the compiler
void error_handler_report(int line, Error_Type error_type, char* format, ...);

// A general function that helps the compiler to recover after encountering a syntax error.
// "eats" tokens from the input until finds a token that is at the start of a statment production rule.
// When finds one, pops every token that is no longer relevant (shifted in the current errorful statement) off the stack,
// and shifts said found token with it's correct starting state onto the updated stack.
// At the end, retrieves the next token from the input.
void error_handler_error_recovery();

// The following function are specific functions for each state for better error reporting & recovery.
// This functions are using the error_handler_error_recovery() function.

// Prints an expected `prog` error message to the user, and pushes a prog entry onto the parse stack
void error_handler_report_expected_prog();

// Prints an expected program name error message to the user, and pushes an id entry onto the parse stack
void error_handler_report_expected_prog_name();

// Prints an expected `:` error message to the user, and pushes a `:` entry onto the parse stack
// with the right goto state according to the state in the action table
void error_handler_report_expected_colon_state_3();
void error_handler_report_expected_colon_state_37();
void error_handler_report_expected_colon_state_46();
void error_handler_report_expected_colon_state_59();

// Prints an expected EOF error message to the user
void error_handler_report_expected_eof();

// Prints an expected `:)` error message to the user
void error_handler_report_expected_smiley();

// Prints an expected identifier error message to the user
void error_handler_report_expected_identifier();

// Prints an expected `;` error message to the user
void error_handler_report_expected_semi_colon();

// Prints an expected `(` error message to the user
void error_handler_report_expected_open_paren();

// Prints an expected `=` error message to the user
void error_handler_report_expected_assign();

// Prints the expected token group of state 4 states group error message to the user.
// In the action table this token group contains: `done`, `int`, `char`, `set`, `if`, `while`.
// States that contain this token group: 4, 6, 8, 9, 10, 11, 22, 47, 48, 55 ,57, 58, 60, 61
void error_handler_report_expected_state_4_group();

// Prints the expected token group of state 7 states group error message to the user.
// In the action table this token group contains: `:)`, `done`, `int`, `char`, `set`, `if`, `else`, `while`.
// States that contain this token group: 7, 17
void error_handler_report_expected_state_7_group();

// Prints the expected token group of state 20 states group error message to the user.
// In the action table this token group contains: id, `(`, `-`, number, character, `!`
// States that contain this token group: 20, 21, 23, 32, 33, 34, 38, 39, 40, 41, 42
void error_handler_report_expected_state_20_group();

// Prints the expected token group of state 24 states group error message to the user.
// In the action table this token group contains: `)`, `||`
// States that contain this token group: 24, 35, 43
void error_handler_report_expected_state_24_group();

// Prints the expected token group of state 25 states group error message to the user.
// In the action table this token group contains: `;`, `)`, binary_operator
// States that contain this token group: 25, 26, 27, 28, 29, 30, 31, 44, 45, 49, 50, 51, 52, 53, 54
void error_handler_report_expected_state_25_group();

// Prints the expected token group of state 36 states group error message to the user.
// In the action table this token group contains: `;`, `||`
// States that contain this token group: 36
void error_handler_report_expected_state_36_group();

// Prints the expected token group of state 56 states group error message to the user.
// In the action table this token group contains: `done`, `int`, `char`, `set`, `if`, `else`, `while`.
// States that contain this token group: 56
void error_handler_report_expected_state_56_group();

// Converts and error code to it's string representation
const char* error_handler_error_to_str(Error_Type error_type);
