#include <stdlib.h>
#include <stdio.h>

#include "include/do.h"

int main()
{
    char code[] = "\tW\t pW_3rogif 1W1 []#23'$';int if:()) ch$archar\n char set0elsei else while set done   ";
    // char code[] = "This is a cool prog! :)";

    do_compile(code);

    return 0;
}
