#include <stdlib.h>

#include "../include/do.h"
#include "../include/io.h"
#include "../include/parser.h"
#include "../include/error_handler.h"

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
