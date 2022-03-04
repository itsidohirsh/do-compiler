#include <stdlib.h>
#include <stdio.h>

#include "../include/parser.h"
#include "../include/error_handler.h"

Parser* parser_init(char* src)
{
    // Create parser
    Parser* parser = (Parser*) calloc(1, sizeof(Parser));

    // Create parser's lexer
    parser->lexer = lexer_init(src);

    return parser;
}

void parser_destroy(Parser* parser)
{
    lexer_destroy(parser->lexer);
    free(parser);
}

void parser_parse(Parser* parser)
{
    Token* token = NULL;
    char* type = NULL;

    // While the token is not EOF
    while ((token = lexer_get_next_token(parser->lexer))->type != Token_Eof)
    {
        if (token->type == Token_Error)
            error_handler_report(parser->lexer->line, "Lexer: Unexpected characters '%s'", 1, (void* []) { (void*) token->value });

        if (token->type == Token_Whitespace)
            continue;

        type = token_to_str(token);
        printf("%s\n", type);
        free(type);

        free(token->value);
        free(token);
    }
}
