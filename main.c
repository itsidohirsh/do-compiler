#include "src/general/general.h"
#include "src/compiler/compiler.h"


int main(int argc, char* argv[])
{
    // If no command line arguments were specified
    if (argc < 2)
    {
        printf("Do: " RED "fatal error:" RESET " no input files\n");
        printf("\tsyntax: " BOLD_WHITE "do <source> [<destination>]\n" RESET);
        return 1;
    }

    // If there are 2 or more arguments, intialize the compiler with the first two
    if (argc >= 3)
        compiler_init(argv[1], argv[2]);

    // If there is 1 argument, intialize the compiler with it as the source and with ./a.asm as the destination
    else
        compiler_init(argv[1], "./a.asm");

    // Compile the source code
    compiler_compile();

    // Destroy the compiler
    compiler_destroy();

    return 0;
}
