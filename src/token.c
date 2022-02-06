#include <stdlib.h>

#include "../include/token.h"

Token* token_init(char* value, Token_Type type)
{
    Token* token = (Token*) calloc(1, sizeof(Token));
    token->value = value;
    token->type = type;

    return token;
}