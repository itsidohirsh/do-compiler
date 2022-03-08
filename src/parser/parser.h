#pragma once

#include "../lexer/lexer.h"
#include "../parse_tree/parse_tree.h"
#include "parsing_table/parsing_table.h"
#include "parser_stack/parser_stack.h"

// Number of production rules in the grammar of the language
#define NUM_OF_PRODUCTION_RULES 28


/* ---------- Structs ---------- */

// A struct to hold each production rule's length and non-terminal on its LHS
typedef struct Production_Rule
{
    Non_Terminal_Type non_terminal_type; // The non-terminal that is on the LHS of a production rule
    int rule_length; // The number of Terminals and Non-Terminals that are on the RHS of a production rule
} Production_Rule;

// Struct of the parser
typedef struct Parser
{
    // The parser's lexer, used to get the next token from the source code
    Lexer* lexer;
    // The parser's parsing table, made from action & goto tables
    // helps up to know which action to perform according to the next token
    Parsing_Table* parsing_table;
    // The parser's stack
    Parser_Stack_Entry* parser_stack;
    // Array of the production rules
    // This will be used when we reduce by a production rule in the parsing phase
    Production_Rule production_rules[NUM_OF_PRODUCTION_RULES];
} Parser;


/* ---------- Functions ---------- */

// Creates a new parser on the heap and returns a pointer to it
Parser* parser_create();

// Frees everything we've allocated in the parser_create() function
void parser_destroy(Parser* parser);

// Initializes the parser. Parsing table & stack
void parser_init(Parser* parser, char* src);

// Parses the source code and returns an Abstract Symtax Tree / Parse Tree that represents the source code
Parse_Tree_Node* parser_parse(Parser* parser, char* src);
