#pragma once

/* ---------- Structs ---------- */

// Enum of all the token types
typedef enum Token_Type
{
    Token_Error,            // Error token, must be 0        
    Token_Whitespace,       // Whitespace characters: ` `, `\n`, `\t`, `\r`, `\v`, `\f`
    Token_Identifier,       // Identifier
    Token_Number,           // Number literal
    Token_Character,        // Character literal
    Token_Int,              // `int`
    Token_Char,             // `char`
    Token_Prog,             // `prog`
    Token_If,               // `if`
    Token_Else,             // `else`
    Token_While,            // `while`
    Token_Set,              // `set`
    Token_Done,             // `done`
    Token_Assignment,       // `=`
    Token_Equal,            // `==`
    Token_Not,              // `!`
    Token_Not_Equal,        // `!=`
    Token_Bigger,           // `>`
    Token_Bigger_Equal,     // `>=`
    Token_Smaller,          // `<`
    Token_Smaller_Equal,    // `<=`
    Token_Or,               // `||`
    Token_And,              // `&&`
    Token_Plus,             // `+`
    Token_Minus,            // `-`
    Token_Multiply,         // `*`
    Token_Divide,           // `/`
    Token_Modulu,           // `%`
    Token_Open_Paren,       // `(`
    Token_Close_Paren,      // `)`
    Token_Colon,            // `:`
    Token_Smiley,           // `:)`
    Token_Semi_Colon,       // `;`
    Token_Eof,              // End Of File token
} Token_Type;

// Token struct
typedef struct Token
{
    char* value;
    int value_len;
    Token_Type type;
} Token;


/* ---------- Functions ---------- */

// Gets a value and a token type, and returns a Token with those inputs
Token* token_init(char* value, int value_len, Token_Type type);

// Converts the Token_Type to its matching type name in the enum of the types of a Token
static const char* token_type_to_str(Token_Type type);

// Creates a string that describes the inputed token
char* token_to_str(Token* token);
