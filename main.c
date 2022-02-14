#include <stdlib.h>
#include <stdio.h>

#include "include/do.h"
#include "include/error_handler.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        red();
        printf("Please specify input file.\n");
        reset();
        return 1;
    }

    do_compile_file(argv[1]);

    return 0;
}
