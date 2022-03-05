#include <stdlib.h>

#include "do.h"
#include "../io/io.h"
#include "../parser/parser.h"

void do_compile(char* src)
{
    Parser* parser = parser_init(src);
    parser_parse(parser);
    parser_destroy(parser);
}

void do_compile_file(const char* filename)
{
    char* src = read_file(filename);
    do_compile(src);
    free(src);
}
