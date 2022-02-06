#pragma once

/* ---------- Structs ---------- */

// Enum of all the token types
typedef enum Token_Type
{
    Token_Error,
    Token_Whitespace,
    Token_Identifier,
    Token_Number,
    Token_Character,
    Token_Int,
    Token_Char,
    Token_Prog,
    Token_If,
    Token_Else,
    Token_While,
    Token_Set,
    Token_Done,
    Token_Assignment,
    Token_Equal,
    Token_Not,
    Token_Not_Equal,
    Token_Bigger,
    Token_Bigger_Equal,
    Token_Smaller,
    Token_Smaller_Equal,
    Token_Or,
    Token_And,
    Token_Plus,
    Token_Minus,
    Token_Multiply,
    Token_Divide,
    Token_Modulu,
    Token_Open_Paren,
    Token_Close_Paren,
    Token_Colon,
    Token_Semi_Colon,
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
