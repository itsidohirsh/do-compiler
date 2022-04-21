#pragma once

#include <stdbool.h>

#include "lexer_fsm/lexer_fsm.h"
#include "../token/token.h"


// The size to allocate for each new token value
#define LEXER_MAX_TOKEN_SIZE 32 // 31 + 1, the max size of a token is 31 characters long. +1 for the null terminator


/* ---------- Structs ---------- */

// Struct of the lexer
typedef struct Lexer
{
    char c;         // Current character in the src code
    int i;          // Current offsest from the starting of the source code
    Lexer_FSM* fsm; // The lexer's FSM
} Lexer;


/* ---------- Functions ---------- */

// Create a new lexer on the heap and updates the compiler's lexer to point to it
void lexer_create();

// Frees everything we've allocated in the lexer_init() function for the compiler's lexer
void lexer_destroy();

// Initializes the compiler's lexer
void lexer_init();

// Advances the lexer 1 character forward in the source code
void lexer_advance();

// Creates and returns a new token when reached to the end of a token
Token* lexer_EOT(char* value, int size, int state);

// Checks whether a token is a skip token according to the given token type.
// The skip toknes are: Whitespace, Comment
bool lexer_is_skip_token(Token_Type type);

// Returns the next token from the source code
Token* lexer_get_next_token();
