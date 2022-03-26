#include <stdlib.h>

#include "../global.h"

#include "../general/general.h"
#include "compiler.h"


void compiler_init(char* src)
{
    // Initialize with 0 errors
    compiler.errors = 0;

    // Source code
    compiler.src = src;

    // Symbol table
    scope_tree_create();

    // Lexer
    lexer_create();
    lexer_init();

    // Parser
    parser_create();
    parser_init();
}

void compiler_destroy()
{
    // Source code
    free(compiler.src);
    compiler.src = NULL;

    // Symbol table
    scope_tree_destroy();

    // Lexer
    lexer_destroy();

    // Parser
    parser_destroy();
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

        // Destroy the compiler
        compiler_destroy();

        printf(RED "\n\nCompilation terminated!\n\n" RESET);
        exit(1);
    }

    // Print the parse tree and symbol table
    parse_tree_print(parse_tree);
    scope_tree_print(compiler.scope_tree->global_scope);

    // Destroy parse tree
    parse_tree_destroy(parse_tree);
}
