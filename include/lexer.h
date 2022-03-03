#pragma once

#include <stdlib.h>

#include "token.h"
#include "lexer_fsm.h"


// The size to allocate for each new token value
#define LEXER_SIZE_OF_TOKEN_VALUE 32


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

// Initializes the lexer
// Creates all neccesery components of the lexer on the heap and returns the lexer
Lexer* lexer_init(char* src);

// Frees everything we've allocated in the lexer_init() function
void lexer_destroy(Lexer* lexer);

// Advances the lexer 1 character forward in the source code
void lexer_advance(Lexer* lexer);

// Advances the lexer and returns inputed token
Token* lexer_advance_with(Lexer* lexer, Token* token);

// Returns the next token from the source code
Token* lexer_get_next_token(Lexer* lexer);
