#include <stdlib.h>
#include <stdio.h>

#include "include/token.h"
#include "include/lexer.h"

int main()
{
    char code[] = "w++ int n = 15;i++; z/15+4='?' if\n(while(hello)) else 123lol";
    Lexer* lexer = lexer_init(code);
    Token* token = NULL;

    lexer_fsm_print(lexer->fsm);

    while ((token = lexer_next_token(lexer))->type != Token_Eof)
    {
        if (token->type == Token_Error)
            printf("[Lexer]: Unexpected character `%c`\n", lexer->c);
        else
            printf("<Value: `%s`, Type: %d>\n", token->value, token->type);

        free(token->value);
        free(token);
        lexer_advance(lexer);
    }

    lexer_destroy(lexer);

    return 0;
}
