#include <stdlib.h>

#include "../global.h"

#include "../general/general.h"
#include "compiler.h"
#include "../io/io.h"
#include "../code_generator/code_generator.h"


void compiler_init(char* src_file_name, char* dest_file_name)
{
    // Source code
    compiler.src = io_read_file(src_file_name);

    // Destination file
    compiler.dest_file_name = dest_file_name;

    // Lexer
    lexer_create();
    lexer_init();

    // Parser
    parser_create();
    parser_init();

    // Symbol table
    scope_tree_create();

    // Code generator
    code_generator_create();
    code_generator_init();

    // Initialize with 0 errors
    compiler.errors = 0;

    // Initialize with line number 1
    compiler.line = 1;
}

void compiler_destroy()
{
    // Source code
    free(compiler.src);
    compiler.src = NULL;

    // Lexer
    lexer_destroy();

    // Parser
    parser_destroy();

    // Symbol table
    scope_tree_destroy();

    // Code generator
    code_generator_destroy();
}

void compiler_compile()
{
    // Parse the source code and create a parse tree that represents it.
    // Also updates the symbol table.
    Parse_Tree_Node* parse_tree = parser_parse();

    // If errors were encountered during compilation
    if (compiler.errors > 0)
    {
        // Destroy parse tree
        parse_tree_destroy(parse_tree);

        // Delete destination file
        remove(compiler.dest_file_name);

        // Destroy the compiler
        compiler_destroy();

        printf(RED "\n\nCompilation terminated.\n" RESET);
        exit(1);
    }

    // Generate the assembly code for the created parse tree
    code_generator_generate(parse_tree);

    // If reached this point, that means the compilation process was successfull!
    printf(BOLD_WHITE "\nParsing...\n");
    printf("Generating code...\n");
    printf(BOLD_GREEN "\nCompilation successful!\n" RESET);

    // Destroy parse tree
    parse_tree_destroy(parse_tree);
}
