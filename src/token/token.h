#pragma once

/* ---------- Structs ---------- */

// Enum of all the token types
typedef enum Token_Kind
{
    Token_Error,            // Error token, must be 0        
    Token_Whitespace,       // Whitespace characters: ` `, `\n`, `\t`, `\r`, `\v`, `\f`
    Token_Prog,             // `prog`
    Token_Identifier,       // Identifier
    Token_Colon,            // `:`
    Token_Smiley,           // `:)`
    Token_Done,             // `done`
    Token_Int,              // `int`
    Token_Char,             // `char`
    Token_Semi_Colon,       // `;`
    Token_Set,              // `set`
    Token_Assignment,       // `=`
    Token_If,               // `if`
    Token_Open_Paren,       // `(`
    Token_Close_Paren,      // `)`
    Token_Else,             // `else`
    Token_While,            // `while`
    Token_Or,               // `||`
    Token_And,              // `&&`
    Token_Equal,            // `==`
    Token_Not_Equal,        // `!=`
    Token_Bigger,           // `>`
    Token_Bigger_Equal,     // `>=`
    Token_Smaller,          // `<`
    Token_Smaller_Equal,    // `<=`
    Token_Plus,             // `+`
    Token_Minus,            // `-`
    Token_Multiply,         // `*`
    Token_Divide,           // `/`
    Token_Modulu,           // `%`
    Token_Number,           // Number literal
    Token_Character,        // Character literal
    Token_Not,              // `!`
    Token_Eof,              // End Of File token
} Token_Kind;

// Token struct
typedef struct Token
{
    char* value;        // The value of the current token from the source code
    int value_len;      // The length of the value
    Token_Kind token_kind;    // The type of the token. From the Token_Kind enum
} Token;


/* ---------- Functions ---------- */

// Gets a value and a token type, and returns a Token with those inputs
Token* token_init(char* value, int value_len, Token_Kind token_kind);

// Frees a token
void token_destroy(Token* token);

// Converts the Token_Kind to its matching token_kind name in the enum of the types of a Token
const char* token_type_to_str(Token_Kind token_kind);

// Creates a string that describes the inputed token
char* token_to_str(Token* token);
