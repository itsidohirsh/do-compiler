#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../error_handler/error_handler.h"

Lexer* lexer_create()
{
    // Create lexer
    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    // Check for allocation error
    if (lexer == NULL)
    {
        lexer_destroy(lexer);
        error_handler_report_alloc();
    }

    // Create lexer's FSM
    lexer->fsm = lexer_fsm_create();

    return lexer;
}

void lexer_destroy(Lexer* lexer)
{
    lexer_fsm_destroy(lexer->fsm);
    free(lexer);
}

void lexer_init(Lexer* lexer, char* src)
{
    // Initialize lexer's components
    lexer->src = src;
    lexer->i = 0;
    lexer->c = lexer->src[lexer->i];
    lexer->line = 1;

    // Initialize lexer's Finite State Machine
    lexer_fsm_init(lexer->fsm);
}

void lexer_advance(Lexer* lexer)
{
    lexer->i++;
    lexer->c = lexer->src[lexer->i];

    // Advance line number if current character is Line Feed / a new line character, '\n'
    if (lexer->c == '\n')
        lexer->line++;
}

Token* lexer_EOT(Lexer* lexer, char* value, int size, int state)
{
    // If the token is an error token then report an error and exit
    if (lexer->fsm->states[state].token_type == Token_Error)
    {
        // Reallocating the value to its actual size
        value = (char*) realloc(value, (size + 1) * sizeof(char));
        // Check for allocation error
        if (value == NULL) error_handler_report_alloc();

        // Making sure there is a null terminator at the end of the value
        value[size] = '\0';

        error_handler_report(lexer->line, "Lexer: Unexpected characters '%s'", 1, (void* []) { (void*) value });
    }

    // If the token is Whitespace, advance and return NULL to indicate we've reached a Whitespace token
    if (lexer->fsm->states[state].token_type == Token_Whitespace)
    {
        lexer_advance(lexer);
        return NULL;
    }

    // Reallocating the value to its actual size
    value = (char*) realloc(value, (size + 1) * sizeof(char));
    // Check for allocation error
    if (value == NULL) error_handler_report_alloc();

    // Making sure there is a null terminator at the end of the value
    value[size] = '\0';

    // Advancing and returning a new token
    lexer_advance(lexer);
    return token_init(value, size, lexer->fsm->states[state].token_type);
}

Token* lexer_get_next_token(Lexer* lexer)
{
    // Current token
    Token* token;

    // Get the starting state according to the current character from the source code
    int state = lexer_fsm_get_starting_state_index(lexer->fsm, lexer->c);

    // The value of the token that will be returned
    char* value = (char*) calloc(LEXER_MAX_TOKEN_SIZE, sizeof(char));
    // Check for allocation error
    if (value == NULL) error_handler_report_alloc();
    // the size of the token's value
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
            token = lexer_EOT(lexer, value, size, state);

            // Return the token only if the lexer_EOT didn't return NULL, because if returned NULL that means it was a Whitespace token an we want to continue
            if (token != NULL)
                return token;

            // If we got Whitespace token, update starting state and size and continue to next token
            state = lexer_fsm_get_starting_state_index(lexer->fsm, lexer->c);
            size = 0;
            continue;
        }

        // Advance to the next state
        state = lexer->fsm->edges[state][lexer_fsm_get_char_index(lexer->src[lexer->i + 1])].state_number;
        lexer_advance(lexer);
    }

    // If we've reached the end of the src code but the value is not empty, return a new token with that value
    if (size > 0)
    {
        token = lexer_EOT(lexer, value, size, state);

        // Return only if the lexer_EOT didn't return NULL, because if NULL it means it's a Whitespace token
        if (token != NULL)
            return token;
    }

    // When we've reached to the end of the src code, return End Of File token
    return token_init(NULL, 0, Token_Eof);
}
