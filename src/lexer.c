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

Token* lexer_advance_with(Lexer* lexer, Token* token)
{
    lexer_advance(lexer);
    return token;
}

Token* lexer_next_token(Lexer* lexer)
{
    // Get the starting state according to the current character from the source code
    int state = lexer_fsm_get_state_index(lexer->fsm, lexer->c);

    Token* token = NULL;

    // The value of the token that will be returned
    char* value = (char*) calloc(32, sizeof(char));
    int size = 0;

    // While not EOS
    while (lexer->c != '\0')
    {
        // Update value
        value[size++] = lexer->c;

        // If there is no state to acvance to according to the current state and input character, return a new token with the value
        if (lexer->fsm->edges[state][lexer_fsm_get_char_index(lexer->src[lexer->i + 1])].weight == 0)
            return lexer_advance_with(lexer, token_init(value, lexer->fsm->states[state].token_type));

        // Advance to the next state
        state = lexer->fsm->edges[state][lexer_fsm_get_char_index(lexer->src[lexer->i + 1])].weight;
        lexer_advance(lexer);
    }

    // If we are at the end of the src code but there is value in value, return a new token with that value
    if (size > 0)
        return lexer_advance_with(lexer, token_init(value, lexer->fsm->states[state].token_type));

    // When we've reached to the end of the src code, return End Of File token
    return token_init(0, Token_Eof);
}
