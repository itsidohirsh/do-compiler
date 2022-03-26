#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "../global.h"

#include "error_handler.h"
#include "../general/general.h"


void error_handler_report(int line, Error_Type error_type, char* format, ...)
{
    printf("\n[" RED "%s" RESET " on line" CYAN " %d" RESET "] ", error_handler_error_to_str(error_type), line);

    va_list args;           // Declare a va_list type variable
    va_start(args, format); // Initialize the va_list with the ...
    vprintf(format, args);  // Forward ... to vprintf
    va_end(args);           // Clean up the va_list

    compiler.errors++;
}

void error_handler_error_recovery()
{
    // Get the starting state of the current token
    int token_starting_state = parser_get_token_starting_state(compiler.parser->token->token_type);

    // Loop until found a token which is a start of a statement, or until reached EOF.
    // Ignore all toknes until findes a token which is a start of a statement.
    while (token_starting_state == 0 && compiler.parser->token->token_type != Token_Eof)
    {
        // Destroy previous token
        token_destroy(compiler.parser->token);
        // Get next token
        compiler.parser->token = lexer_get_next_token();
        // Get the starting state of the newly fetched token
        token_starting_state = parser_get_token_starting_state(compiler.parser->token->token_type);
    }

    // Pop not needed, previously shifted tokens of the current part of the block off the parse stack.
    // Stop if only one entry left in the parse stack (the init entry).
    Parse_Stack_Entry* entry = parse_stack_pop();
    while (compiler.parser->parse_stack->next_entry != NULL)
    {
        // If reached the start of the current block that means we've poped enough off the stack and can continue parsing.
        if (entry->tree->symbol_type == Terminal && entry->tree->token->token_type == Token_Colon)
            break;

        // Destroy poped entry
        parse_tree_destroy(entry->tree);
        free(entry);

        // Pop the next entry
        entry = parse_stack_pop();
    }

    // Push back last poped entry (becasue we're popping one too many entries)
    parse_stack_push(entry);

    // Shift the found token with it's starting state
    parser_shift(token_starting_state);

    // Get next token from the source code
    compiler.parser->token = lexer_get_next_token();
}

void error_handler_report_expected_prog()
{
    Token_Type type = Token_Prog;
    int goto_state = 2;

    printf(", expected '" BOLD_WHITE "prog" RESET "' at the start of the program");

    parse_stack_push(parse_stack_init_entry(parse_tree_init_node(Terminal, type, token_init(NULL, 0, type), NULL, 0), goto_state));
}

void error_handler_report_expected_prog_id()
{
    Token_Type type = Token_Identifier;
    int goto_state = 3;

    printf(", expected " BOLD_WHITE "program name" RESET " after '" BOLD_WHITE "prog" RESET "'");

    parse_stack_push(parse_stack_init_entry(parse_tree_init_node(Terminal, type, token_init(NULL, 0, type), NULL, 0), goto_state));
}

void error_handler_report_expected_colon_state_3()
{
    Token_Type type = Token_Colon;
    int goto_state = 4;

    printf(", expected '" BOLD_WHITE ":" RESET "' after " BOLD_WHITE "program name" RESET);

    parse_stack_push(parse_stack_init_entry(parse_tree_init_node(Terminal, type, token_init(NULL, 0, type), NULL, 0), goto_state));
}

void error_handler_report_expected_colon_state_37()
{
    Token_Type type = Token_Colon;
    int goto_state = 48;

    printf(", expected '" BOLD_WHITE ":" RESET "' at the start of an if block");

    parse_stack_push(parse_stack_init_entry(parse_tree_init_node(Terminal, type, token_init(NULL, 0, type), NULL, 0), goto_state));
}

void error_handler_report_expected_colon_state_46()
{
    Token_Type type = Token_Colon;
    int goto_state = 55;

    printf(", expected '" BOLD_WHITE ":" RESET "' at the start of a while block");

    parse_stack_push(parse_stack_init_entry(parse_tree_init_node(Terminal, type, token_init(NULL, 0, type), NULL, 0), goto_state));
}

void error_handler_report_expected_colon_state_59()
{
    Token_Type type = Token_Colon;
    int goto_state = 60;

    printf(", expected '" BOLD_WHITE ":" RESET "' at the start of an else block");

    parse_stack_push(parse_stack_init_entry(parse_tree_init_node(Terminal, type, token_init(NULL, 0, type), NULL, 0), goto_state));
}


void error_handler_report_expected_semi_colon()
{
    printf(", expected '" BOLD_WHITE ";" RESET "'");

    error_handler_error_recovery();
}

void error_handler_report_expected_identifier()
{
    printf(", expected " BOLD_WHITE "identifier" RESET);

    error_handler_error_recovery();
}

void error_handler_report_expected_open_paren()
{
    printf(", expected '" BOLD_WHITE "(" RESET "'");

    error_handler_error_recovery();
}

void error_handler_report_expected_assign()
{
    printf(", expected '" BOLD_WHITE "=" RESET "'");

    error_handler_error_recovery();
}


const char* error_handler_error_to_str(Error_Type error_type)
{
    switch (error_type)
    {
        case Error_Lexical: return "Lexical Error";
        case Error_Syntax: return "Syntax Error";
        case Error_Semantic: return "Semantic Error";

        default: return "Error";
    }
}
