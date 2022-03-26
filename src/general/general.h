#pragma once

#include <stdio.h>


/* ---------- Marcros ---------- */

// Used to printf with colors
#define RESET           "\033[0m"
#define BLACK           "\033[30m"              /* Black */
#define RED             "\033[31m"              /* Red */
#define GREEN           "\033[32m"              /* Green */
#define YELLOW          "\033[33m"              /* Yellow */
#define BLUE            "\033[34m"              /* Blue */
#define PURPLE          "\033[35m"              /* Purple */
#define CYAN            "\033[36m"              /* Cyan */
#define WHITE           "\033[37m"              /* White */
#define BOLD_BLACK      "\033[1m\033[30m"       /* Bold Black */
#define BOLD_RED        "\033[1m\033[31m"       /* Bold Red */
#define BOLD_GREEN      "\033[1m\033[32m"       /* Bold Green */
#define BOLD_YELLOW     "\033[1m\033[33m"       /* Bold Yellow */
#define BOLD_BLUE       "\033[1m\033[34m"       /* Bold Blue */
#define BOLD_MAGENTA    "\033[1m\033[35m"       /* Bold Magenta */
#define BOLD_CYAN       "\033[1m\033[36m"       /* Bold Cyan */
#define BOLD_WHITE      "\033[1m\033[37m"       /* Bold White */


/* ---------- Functions ---------- */

// Expecting __FILE__ and __LINE__ to report a memory allocation error.
// Reports an memory allocation error, destorys the global compiler and exits with exit code 1.
void exit_memory_error(char* file, int line);

// Expecting the file name that was not able to perform I/O well to report a file I/O error.
// Reports an file I/O error, destorys the global compiler and exits with exit code 1.
void exit_file_io_error(char* file_name);
