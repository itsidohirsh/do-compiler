#pragma once


// printf in different colors
void printf_red(const char* format, ...);
void printf_cyan(const char* format, ...);

// Reports an error message to the user and exits
void error_handler_report(int line, char* format, ...);

// Reports an allocation error message to the user and exits
void error_handler_report_memory_error();

// Reports a file I/O error message to the user and exits
void error_handler_report_file_error(const char* filename);
