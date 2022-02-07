#include <stdlib.h>
#include <stdio.h>

#include "include/do.h"

int main()
{
    char code[] = "\t\t progif 123'A'int if charchar char elsei else while set done";

    do_compile(code);

    return 0;
}
