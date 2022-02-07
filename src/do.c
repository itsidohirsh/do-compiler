#include <stdlib.h>
#include <stdio.h>

#include "../include/do.h"
#include "../include/lexer.h"
#include "../include/token.h"

void do_compile(char* src)
{
    Lexer* lexer = lexer_init(src);
    Token* token = NULL;

    // While the token is not EOF
    while ((token = lexer_next_token(lexer))->type != Token_Eof)
    {
        if (token->type != Token_Error)
            printf("%s\n", token_to_str(token));
        else
            printf("[Lexer]: Unexpected character `%c`\n", lexer->c);

        free(token->value);
        free(token);
    }

    lexer_destroy(lexer);
}
