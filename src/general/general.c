#include <stdlib.h>

#include "general.h"
#include "../compiler/compiler.h"


void exit_memory_error(char* file, int line)
{
    printf("[" RED "Memory Error" RESET "] Memory allocation failed.\n");
    printf("\tIn file: " BOLD_WHITE "%s\n" RESET, file);
    printf("\tOn line: " CYAN "%d\n" RESET, line);

    compiler_destroy();
    exit(1);
}

void exit_file_io_error(char* file_name)
{
    printf("[" RED "File I/O Error" RESET "] Could not read file " BOLD_WHITE "%s\n" RESET, file_name);

    compiler_destroy();
    exit(1);
}
