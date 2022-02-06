#pragma once

#include "token.h"
#include "lexer_fsm.h"


/* ---------- Structs ---------- */

// Struct of the lexer
typedef struct Lexer
{
    char* src;
    size_t src_len;
    char c;
    size_t i;
    Lexer_FSM* fsm;
} Lexer;


/* ---------- Functions ---------- */

// Initializes the lexer
// Creates all neccesery components of the lexer on the heap and returns the lexer
Lexer* lexer_init(char* src);

// Frees everything we've allocated in the lexer_init() function
void lexer_destroy(Lexer* lexer);

// Advances the lexer 1 character on the source code
void lexer_advance(Lexer* lexer);

// Returns the next token from the source code
Token* lexer_next_token(Lexer* lexer);
