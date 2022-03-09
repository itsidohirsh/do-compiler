#include "do.h"
#include "../io/io.h"
#include "../parser/parser.h"


void do_compile(char* src)
{
    Parser* parser = parser_create();
    Parse_Tree_Node* parse_tree = parser_parse(parser, src);
    parse_tree_print_tree(parse_tree);
    parse_tree_destroy_tree(&parse_tree);
}

void do_compile_file(const char* filename)
{
    char* src = read_file(filename);
    do_compile(src);
    free(src);
}
