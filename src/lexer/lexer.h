#pragma once

#include <stdlib.h>

#include "../token/token.h"
#include "lexer_fsm/lexer_fsm.h"


// The size to allocate for each new token value
#define LEXER_MAX_TOKEN_SIZE 33 // 32 + 1, the max size of a atoken is 32 characters long. +1 for the null terminator


/* ---------- Structs ---------- */

// Struct of the lexer
typedef struct Lexer
{
    char* src;      // The src code
    char c;         // Current character in the src code
    size_t i;       // Current offsest from the starting of the source code
    size_t line;    // Current line number in source file for error reporting
    Lexer_FSM* fsm; // The lexer's FSM
} Lexer;


/* ---------- Functions ---------- */

// Create a new lexer on the heap and returns a pointer to it
Lexer* lexer_create();

// Frees everything we've allocated in the lexer_init() function
void lexer_destroy(Lexer* lexer);

// Initializes the lexer
void lexer_init(Lexer* lexer, char* src);

// Advances the lexer 1 character forward in the source code
void lexer_advance(Lexer* lexer);

// Creates and returns a new token when reached to the end of a token
Token* lexer_EOT(Lexer* lexer, char* value, int size, int state);

// Returns the next token from the source code
Token* lexer_get_next_token(Lexer* lexer);
