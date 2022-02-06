#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/lexer.h"

Lexer* lexer_init(char* src)
{
    // Create lexer
    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    lexer->src = src;
    lexer->src_len = strlen(src);
    lexer->i = 0;
    lexer->c = lexer->src[lexer->i];
    // Create lexer FSM
    lexer->fsm = (Lexer_FSM*) calloc(1, sizeof(Lexer_FSM));
    lexer_fsm_init(lexer->fsm);

    return lexer;
}

void lexer_destroy(Lexer* lexer)
{
    // Free lexer's FSM
    free(lexer->fsm);
    // Free the lexer
    free(lexer);
}

void lexer_advance(Lexer* lexer)
{
    if (lexer->i < lexer->src_len && lexer->c != '\0')
    {
        lexer->i++;
        lexer->c = lexer->src[lexer->i];
    }
}
