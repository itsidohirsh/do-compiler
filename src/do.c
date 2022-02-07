#include <stdlib.h>
#include <stdio.h>

#include "../include/do.h"
#include "../include/lexer.h"
#include "../include/token.h"

void do_compile(char* src)
{
    Lexer* lexer = lexer_init(src);
    Token* token = NULL;
    char* type = NULL;

    // While the token is not EOF
    while ((token = lexer_next_token(lexer))->type != Token_Eof)
    {
        if (token->type != Token_Error)
        {
            type = token_to_str(token);
            printf("%s\n", type);
            free(type);
        }
        else
            printf("[Lexer]: Unexpected character `%c`\n", lexer->c);

        free(token->value);
        free(token);
    }

    lexer_destroy(lexer);
}
