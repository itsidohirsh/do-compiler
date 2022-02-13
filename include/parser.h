#pragma once

#include "lexer.h"

/* ---------- Structs ---------- */

// Struct of the parser
typedef struct Parser
{
    Lexer* lexer;
} Parser;



/* ---------- Functions ---------- */

// Initializes the parser
// Creates all neccesery components of the parser on the heap and returns the parser
Parser* parser_init(char* src);

// Frees everything we've allocated in the parser_init() function
void parser_destroy(Parser* parser);

// 
void parser_parse(Parser* parser);
