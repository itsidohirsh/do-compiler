#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/token.h"

Token* token_init(char* value, Token_Type type)
{
    Token* token = (Token*) calloc(1, sizeof(Token));
    token->value = value;
    token->type = type;

    return token;
}

static const char* token_type_to_str(Token_Type type)
{
    switch (type)
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
        case Token_Semi_Colon: return "Semi_Colon";
        case Token_Eof: break;
    }

    // If the type is not one of the values in the enum of types in Token
    return "Don't know that type... 🤔";
}

char* token_to_str(Token* token)
{
    const char* type_str = token_type_to_str(token->type);
    const char* template = "%s: `%s`";

    char* str = (char*) calloc(strlen(type_str) + strlen(template) + 8, sizeof(char));
    sprintf(str, template, type_str, token->value);

    return str;
}
