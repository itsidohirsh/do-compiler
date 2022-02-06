#include <stdlib.h>
#include <stdio.h>

#include "include/token.h"
#include "include/lexer.h"

int main()
{
    char code[] = "prog prog wow";
    Lexer* lexer = lexer_init(code);
    Token* token = NULL;

    lexer_fsm_print(lexer->fsm);

    // while ((token = lexer_next_token(lexer))->type != Token_Eof)
    // {
    //     if (token->type == Token_Error)
    //         printf("[Lexer]: Unexpected character `%c`\n", lexer->c);
    //     else
    //         printf("<Value: `%s`, Type: %d>\n", token->value, token->type);

    //     free(token->value);
    //     free(token);
    //     lexer_advance(lexer);
    // }

    lexer_destroy(lexer);

    return 0;
}
