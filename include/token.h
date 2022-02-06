#pragma once

/* ---------- Structs ---------- */

// Enum of all the token types
typedef enum Token_Type
{
    Token_Error,
    Token_Identifier,
    Token_Prog,
    Token_Assignment,
    Token_Equal,
    Token_Eof,
} Token_Type;

// Token struct
typedef struct Token
{
    char* value;
    Token_Type type;
} Token;


/* ---------- Functions ---------- */

// Gets a value and a token type, and returns a Token with those inputs
Token* token_init(char* value, Token_Type type);