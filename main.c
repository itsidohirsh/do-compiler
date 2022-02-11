#include <stdlib.h>
#include <stdio.h>

#include "include/do.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf("Please specify input file.\n");
        return 1;
    }

    do_compile_file(argv[1]);

    return 0;
}
