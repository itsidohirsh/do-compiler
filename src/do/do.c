#include "do.h"
#include "../io/io.h"
#include "../parser/parser.h"
#include "../symbol_table/scope_tree/scope_tree.h"


void do_compile(char* src)
{
    // Create symbol tables scopes tree
    Scope_Tree* scope_tree = scope_tree_create();

    // Create parser
    Parser* parser = parser_create();

    // Parse the source code and create a parse tree that represents it.
    // Also updates the symbol table.
    Parse_Tree_Node* parse_tree = parser_parse(parser, src, scope_tree);

    // Print the parse tree and symbol table
    parse_tree_print(parse_tree);
    symbol_table_print(scope_tree->global_scope->symbol_table);

    // Free all allocated memory
    parse_tree_destroy(parse_tree);
    scope_tree_destroy(scope_tree);
}

void do_compile_file(char* file_name)
{
    // Read the source code file contents into a buffer
    char* src = io_read_file(file_name);

    // Compile the source code
    do_compile(src);

    // Free the buffer
    free(src);
}
