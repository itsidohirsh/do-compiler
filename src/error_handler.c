#include <stdlib.h>
#include <stdio.h>

#include "../include/error_handler.h"

void red() { printf("\033[0;31m"); }
void cyan() { printf("\033[0;36m"); }
void reset() { printf("\033[0m"); }

void error_handler_report(int line, char* msg, int argc, void* argv[])
{
    printf("[");
    red();
    printf("Error: ");
    reset();
    printf("on line");
    cyan();
    printf(" %d", line);
    reset();
    printf("] ");

    // Print msg according to the number of parameters
    if (argc == 0)
        printf(msg);

    if (argc == 1)
        printf(msg, argv[0]);

    if (argc == 2)
        printf(msg, argv[0], argv[1]);

    printf("\n");

    exit(1);
}
