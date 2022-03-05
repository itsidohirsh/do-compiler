#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../error_handler/error_handler.h"

Lexer* lexer_init(char* src)
{
    // Create lexer
    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    lexer->src = src;
    lexer->i = 0;
    lexer->c = lexer->src[lexer->i];
    lexer->line = 1;

    // Create lexer's FSM
    lexer->fsm = (Lexer_FSM*) calloc(1, sizeof(Lexer_FSM));
    lexer_fsm_init(lexer->fsm);

    return lexer;
}

void lexer_destroy(Lexer* lexer)
{
    free(lexer->fsm);
    free(lexer);
}

void lexer_advance(Lexer* lexer)
{
    lexer->i++;
    lexer->c = lexer->src[lexer->i];

    // Advance line number if current character is Line Feed '\n'
    if (lexer->c == '\n')
        lexer->line++;
}

Token* lexer_advance_with(Lexer* lexer, Token* token)
{
    lexer_advance(lexer);
    return token;
}

Token* lexer_get_next_token(Lexer* lexer)
{
    // Get the starting state according to the current character from the source code
    int state = lexer_fsm_get_starting_state_index(lexer->fsm, lexer->c);

    // The value of the token that will be returned
    char* value = (char*) calloc(LEXER_MAX_TOKEN_SIZE, sizeof(char));
    int size = 0;

    // While not EOS
    while (lexer->c != '\0')
    {
        // Check for Token's max length
        if (size == LEXER_MAX_TOKEN_SIZE - 1)
            error_handler_report(lexer->line, "Lexer: Token can't be longer than %d characters", 1, (void* []) { (void*) (LEXER_MAX_TOKEN_SIZE - 1) });

        // Update value
        value[size++] = lexer->c;

        // If there is no state to advance to according to the current state and input character, return a new token with the value
        if (lexer->fsm->edges[state][lexer_fsm_get_char_index(lexer->src[lexer->i + 1])].state_number == 0)
        {
            value = (char*) realloc(value, (size + 1) * sizeof(char)); // Reallocating the value to its actual size
            value[size] = '\0'; // Making sure there is a null terminator
            return lexer_advance_with(lexer, token_init(value, size, lexer->fsm->states[state].token_type));
        }

        // Advance to the next state
        state = lexer->fsm->edges[state][lexer_fsm_get_char_index(lexer->src[lexer->i + 1])].state_number;
        lexer_advance(lexer);
    }

    // If we've reached the end of the src code but there is value in value, return a new token with that value
    if (size > 0)
    {
        value = (char*) realloc(value, (size + 1) * sizeof(char)); // Reallocating the value to its actual size
        value[size] = '\0'; // Making sure there is a null terminator
        return lexer_advance_with(lexer, token_init(value, size, lexer->fsm->states[state].token_type));
    }

    // When we've reached to the end of the src code, return End Of File token
    return token_init(NULL, 0, Token_Eof);
}
