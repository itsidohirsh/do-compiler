#pragma once


/* ---------- Functions ---------- */

// Expecting __LINE__ and __FILE__ to report a memory allocation error.
// Reports an memory allocation error, destorys the global compiler and exits with exit code 1.
void exit_memory_error(char* file, int line);

// Expecting the file name that was not able to perform I/O well to report a file I/O error.
// Reports an file I/O error, destorys the global compiler and exits with exit code 1.
void exit_file_io_error(char* file_name);

// printf in different colors
void printf_red(const char* format, ...);
void printf_purple(const char* format, ...);
void printf_cyan(const char* format, ...);
