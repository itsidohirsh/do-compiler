#include "src/io/io.h"
#include "src/compiler/compiler.h"
#include "src/error_handler/error_handler.h"


int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf_red("Please specify source file.\n");
        return 1;
    }

    // Read the source code file contents into a buffer
    char* src = io_read_file(argv[1]);

    // Initialize the compiler
    compiler_init(src);

    // Compile the source code
    compiler_compile();

    // Destroy the compiler
    compiler_destroy();

    return 0;
}
