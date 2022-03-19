#include <stdlib.h>
#include <string.h>

#include "../global.h"

#include "lexer.h"
#include "../general/general.h"
#include "../error_handler/error_handler.h"


void lexer_create()
{
    // Create lexer
    compiler.lexer = (Lexer*) calloc(1, sizeof(Lexer));
    if (compiler.lexer == NULL) exit_memory_error(__FILE__, __LINE__);

    // Create lexer's FSM
    lexer_fsm_create();
}

void lexer_destroy()
{
    // check for NULL pointer
    if (compiler.lexer != NULL)
    {
        // Free the lexer's Finite State Machine
        lexer_fsm_destroy();

        // Free the lexer
        free(compiler.lexer);
        compiler.lexer = NULL;
    }
}

void lexer_init()
{
    // Initialize lexer's components
    compiler.lexer->i = 0;
    compiler.lexer->c = compiler.src[compiler.lexer->i];
    compiler.lexer->line = 1;

    // Initialize lexer's Finite State Machine
    lexer_fsm_init(compiler.lexer->fsm);
}

void lexer_advance()
{
    // Advance to the next character in the source code
    compiler.lexer->i++;
    compiler.lexer->c = compiler.src[compiler.lexer->i];

    // Advance line number if current character is Line Feed / a new line character, '\n'
    if (compiler.lexer->c == '\n')
        compiler.lexer->line++;
}

Token* lexer_EOT(char* value, int size, int state)
{
    // If the token is an error token then report an error and exit
    if (compiler.lexer->fsm->states[state].token_type == Token_Error)
    {
        // Copying the value to a buffer so that we can free it.
        char buffer[LEXER_MAX_TOKEN_SIZE];
        strcpy(buffer, value);
        buffer[size] = '\0';

        // Free the value
        free(value);

        // Report an error and exit
        error_handler_report(compiler.lexer->line, Error_Lexical, "Unexpected characters '%s'", buffer);
    }

    // Reallocating the value to its actual size
    value = (char*) realloc(value, (size + 1) * sizeof(char));
    if (value == NULL) exit_memory_error(__FILE__, __LINE__);

    // Making sure there is a null terminator at the end of the value
    value[size] = '\0';

    // Advancing and returning a new token
    lexer_advance();
    return token_init(value, size, compiler.lexer->fsm->states[state].token_type);
}

Token* lexer_get_next_token()
{
    // Get the starting state according to the current character from the source code
    int state = lexer_fsm_get_starting_state_index(compiler.lexer->c);

    // The value of the token that will be returned
    char* value = (char*) calloc(LEXER_MAX_TOKEN_SIZE, sizeof(char));
    if (value == NULL) exit_memory_error(__FILE__, __LINE__);

    // The size of the token's value
    int size = 0;

    // While not EOS
    while (compiler.lexer->c != '\0')
    {
        // Check for Token's max length
        if (size == LEXER_MAX_TOKEN_SIZE - 1)
            error_handler_report(compiler.lexer->line, Error_Lexical, "Token can't be longer than %d characters", LEXER_MAX_TOKEN_SIZE - 1);

        // Update value
        value[size++] = compiler.lexer->c;

        // If there is no state to advance to according to the current state and input character, return a new token with the value
        if (compiler.lexer->fsm->edges[state][lexer_fsm_get_char_index(compiler.src[compiler.lexer->i + 1])].state_number == 0)
        {
            // If the current token is not a Whitespace token, return it
            if (compiler.lexer->fsm->states[state].token_type != Token_Whitespace)
                return lexer_EOT(value, size, state);

            // If the current token is a Whitespace token, continue to the next token like just started the function
            lexer_advance();
            state = lexer_fsm_get_starting_state_index(compiler.lexer->c);
            size = 0;
        }
        // If not EOT yet, advance to the next state
        else
        {
            lexer_advance();
            state = compiler.lexer->fsm->edges[state][lexer_fsm_get_char_index(compiler.lexer->c)].state_number;
        }
    }

    // If we've reached the end of the src code but the value is not empty, and the token is not Whitespace token, then return a new token with that value
    if (size > 0 && compiler.lexer->fsm->states[state].token_type != Token_Whitespace)
        return lexer_EOT(value, size, state);

    // When we've reached to the end of the src code, return End Of File token
    return token_init(NULL, 0, Token_Eof);
}
