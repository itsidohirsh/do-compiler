#include <stdlib.h>
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

Token* lexer_next_token(Lexer* lexer)
{
    // i = index of the current character in the source code
    int i = lexer_fsm_get_index(lexer->c);
    // value = the substring from the source code that is the current token value
    char* value = (char*) calloc(32, sizeof(char));
    // size of the value 
    int size = 0;

    // While not EOS
    while (lexer->c != '\0')
    {
        // Update current token's value
        value[size++] = lexer->c;

        // If there is no edge between curren character and next character OR the current state is a final state, return current token
        if (lexer->fsm->edges[i][lexer_fsm_get_index(lexer->src[lexer->i + 1])].weight == 0 || lexer->fsm->states[i].is_final_state)
            return token_init(value, lexer->fsm->states[lexer_fsm_get_index(lexer->c)].token_type);

        // Go to next state
        i = lexer_fsm_get_index(lexer->fsm->edges[i][lexer_fsm_get_index(lexer->src[lexer->i + 1])].weight);
        lexer_advance(lexer);
    }

    // If we are the the end of the source code but there is a value to the current token, return the token
    if (size != 0)
        return token_init(value, lexer->fsm->states[lexer_fsm_get_index(lexer->src[lexer->i - 1])].token_type);

    // Return EOF token
    return token_init(0, Token_Eof);
}
