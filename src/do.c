#include <stdlib.h>
#include <stdio.h>

#include "../include/do.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/error_handler.h"

void do_compile(char* src)
{
    Lexer* lexer = lexer_init(src);
    Token* token = NULL;
    char* type = NULL;

    // While the token is not EOF
    while ((token = lexer_next_token(lexer))->type != Token_Eof)
    {
        if (token->type == Token_Error)
            error_handler_report(lexer->line, "Unexpected character '%s'", 1, (void* []) { (void*) token->value });

        type = token_to_str(token);
        printf("%s\n", type);
        free(type);

        free(token->value);
        free(token);
    }

    lexer_destroy(lexer);
}

void do_compile_file(const char* filename)
{
    char* src = read_file(filename);
    do_compile(src);
    free(src);
}
