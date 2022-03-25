#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "../general/general.h"


Token* token_init(char* value, int value_len, Token_Type token_type)
{
    // Create a new token
    Token* token = (Token*) calloc(1, sizeof(Token));
    if (token == NULL)
    {
        free(value);
        exit_memory_error(__FILE__, __LINE__);
    }

    // Update token's properties
    token->value = value;
    token->value_len = value_len;
    token->token_type = token_type;

    return token;
}

void token_destroy(Token* token)
{
    // check for NULL pointer
    if (token != NULL)
    {
        // Free the memory allocated for the token's value
        free(token->value);

        // Free the token
        free(token);
    }
}

const char* token_type_to_str(Token_Type token_type)
{
    switch (token_type)
    {
        case Token_Whitespace: return "Whitespace";
        case Token_Identifier: return "Identifier";
        case Token_Number: return "Number";
        case Token_Character: return "Character";
        case Token_Int: return "Int";
        case Token_Char: return "Char";
        case Token_Prog: return "Prog";
        case Token_If: return "If";
        case Token_Else: return "Else";
        case Token_While: return "While";
        case Token_Set: return "Set";
        case Token_Done: return "Done";
        case Token_Assignment: return "Assignment";
        case Token_Equal: return "Equal";
        case Token_Not: return "Not";
        case Token_Not_Equal: return "Not_Equal";
        case Token_Bigger: return "Bigger";
        case Token_Bigger_Equal: return "Bigger_Equal";
        case Token_Smaller: return "Smaller";
        case Token_Smaller_Equal: return "Smaller_Equal";
        case Token_Or: return "Or";
        case Token_And: return "And";
        case Token_Plus: return "Plus";
        case Token_Minus: return "Minus";
        case Token_Multiply: return "Multiply";
        case Token_Divide: return "Divide";
        case Token_Modulu: return "Modulu";
        case Token_Open_Paren: return "Open_Paren";
        case Token_Close_Paren: return "Close_Paren";
        case Token_Colon: return "Colon";
        case Token_Smiley: return "Smiley";
        case Token_Semi_Colon: return "Semi_Colon";
        case Token_Eof: return "EOF";

        default: return "Don't know that token type... ;|";
    }
}

char* token_to_str(Token* token)
{
    const char* type_str = token_type_to_str(token->token_type);

    char* str = (char*) calloc(strlen(type_str) + 16, sizeof(char));
    if (str == NULL)
    {
        token_destroy(token);
        exit_memory_error(__FILE__, __LINE__);
    }

    if (token->token_type == Token_Eof)
        sprintf(str, "%s", type_str);
    else
        sprintf(str, "%s `%s`", type_str, token->value);

    return str;
}
